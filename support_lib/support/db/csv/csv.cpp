/*******************************************************************************
 *
 * Support Library (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

/**************************************************************
 * CSV (c) by Gerhard W. Gruber 2014
 *
 * DESCRIPTION: This module is an implementation of a CSV reader
 * and writer.
 */

#include <iostream>
#include <sys/stat.h>

#include "support/db/type_helper.h"
#include "support/db/csv/csv.h"

const char *ErrorTextArray[] =
{
	"UKNOWN ERROR CODE",
	"EOF",
	"OK",
	"FILE_NOT_OPEN",
	"FILE_ALREADY_OPEN",
	"FILE_BAD_STREAM",
	"FILE_STREAM_ERROR",
	"READ_PARSE_ROW_ERROR",
	"NO_HEADER",
	"NO_COLUMNS_FOUND",
	"COLUMN_FIELD_ERROR",
	"DATA_FIELD_ERROR",
	"DATA_COLUMN_MISSMATCH",
	"NOT_SUPPORTED",
	NULL
};

static bool initErrors(void);
static std::vector<const char *>ErrorText;
static bool init = initErrors();

static bool initErrors(void)
{
	int i = 0;
	while(ErrorTextArray[i] != NULL)
	{
		ErrorText.push_back(ErrorTextArray[i]);
		i++;
	}

	return true;
}

CSV::CSV()
{
	mAutoSeparator = true;
	mStructAdress = NULL;

	setBracket('"', '"');
	setHeader(true);
	setSeparator(0);
	setSampleRowCount(DEFAULT_SAMPLE_ROWS);
}

CSV::~CSV()
{
	close();
	clearColumns();
}

const char *CSV::toErrorText(CSV::ErrorCode nCode)
{
	int i = nCode-2;

	i *= -1;

	if(i < 0 || static_cast<std::size_t>(i) > ErrorText.size())
		return ErrorText[0];

	return ErrorText[i];
}

CSVString const CSV::getFilename(void) const
{
	return mFilename;
}

void CSV::setFilename(CSVString const &oFilename)
{
	mFilename = oFilename;
}

void CSV::setSampleRowCount(unsigned int oSampleNumber)
{
	mSampleRows = oSampleNumber;
}

unsigned int CSV::getSampleRowCount(void) const
{
	return mSampleRows;
}

void CSV::setHeader(bool bHeader)
{
	mHeader = bHeader;
}

bool CSV::hasHeader(void) const
{
	return mHeader;
}

void CSV::setSeparator(CSVChar oSeparator)
{
	mSeparator = oSeparator;
}

CSVChar CSV::getSeparator(void) const
{
	return mSeparator;
}

void CSV::setBracket(CSVChar oOpen, CSVChar oClose)
{
	mOpener = oOpen;
	mCloser = oClose;
}

CSVChar CSV::getBracket(CSVChar &oClose) const
{
	oClose = mCloser;
	return mOpener;
}

bool CSV::isAutoSeparator(void) const
{
	return mAutoSeparator;
}

bool CSV::isSeparator(CSVChar oChar)
{
	CSVChar c = getSeparator();
	if(c != 0)
	{
		if(oChar == c)
			return true;
		else
			return false;
	}

	if(isalpha(oChar))
		return false;

	if(oChar >= '0' && oChar <= '9')
		return false;

	if(oChar == '_')
		return false;

	if(oChar == '#')
		return false;

	return true;
}

bool CSV::isOpen(void)
{
	return mCSVFile.is_open();
}

CSV::ErrorCode CSV::open(CSV::Openmode &oMode)
{
	if(isOpen())
		return CSV::ErrorCode::FILE_ALREADY_OPEN;

	std::ios::openmode md = static_cast<std::ios::openmode>(0);
	if(oMode.flags.READ)
		md |= std::ios::in;

	if(oMode.flags.WRITE)
		md |= std::ios::out;

	if(oMode.flags.BINARY)
		md |= std::ios::binary;

	if(oMode.flags.AT_END)
		md |= std::ios::ate;

	if(oMode.flags.APPEND)
		md |= std::ios::app;

	if(oMode.flags.TRUNCATE)
		md |= std::ios::trunc;

	struct stat fileinfo;
	bool exists = (stat(getFilename().c_str(), &fileinfo) == 0);

	// If the file doesn't exist and we should create it,
	// we have to specify the out flag on a readable file
	// or the truncate mode on a writeable file.
	// If both of them are set it doesn't matter, because
	// the file will be created anyway.
	if(!exists)
	{
		if(oMode.flags.CREATE)
		{
			if(oMode.flags.WRITE)
				md |= (std::ios::out | std::ios::trunc);

			if(oMode.flags.READ)
				md |= std::ios::trunc;
		}
		else
			return CSV::ErrorCode::FILE_NOT_OPEN;
	}

//	md |= std::ios::binary;
	mCSVFile.open(getFilename(), md);
	if(!isOpen())
		return CSV::ErrorCode::FILE_NOT_OPEN;

	if(md & std::ios::trunc)
		writeHeader();
	else
	{
		clearColumns();
		readColumns();
	}

	return CSV::ErrorCode::CSV_OK;
}

void CSV::close(void)
{
	if(isOpen())
		mCSVFile.close();

	if(isAutoSeparator())
		setSeparator(0);

	clearColumns();
}

CSV::ErrorCode CSV::rewind(bool bAbsolute)
{
	if(!isOpen())
		return CSV::ErrorCode::FILE_NOT_OPEN;

	mCSVFile.clear();
	mCSVFile.seekg(0, std::ios_base::beg);
	if(mCSVFile.bad() || mCSVFile.fail())
		return CSV::ErrorCode::FILE_STREAM_ERROR;

	mCSVFile.clear();

	if(!bAbsolute)
	{
		if(hasHeader())
		{
			CSVString line;
			readLine(line);
		}
	}

	return CSV::ErrorCode::CSV_OK;
}

std::vector<CSVColumn *> CSV::getColumns(void)
{
	return mColumns;
}

void CSV::setColumns(std::vector<CSVColumn *> const &oColumns)
{
	clearColumns();

	for(CSVColumn * const &col : oColumns)
		addColumn(col->duplicate());
}

void CSV::addColumn(CSVColumn *oColumn, int nPosition)
{
	if(oColumn == NULL)
		return;

	if(nPosition == -1)
	{
		mColumns.push_back(oColumn);
		return;
	}

	mColumns.insert(mColumns.begin()+nPosition,  oColumn);
}

void CSV::clearColumns(void)
{
	for(CSVColumn *&c : mColumns)
	{
		if(c)
			delete c;
	}

	mColumns.clear();
}

CSV::ErrorCode CSV::count(unsigned int &nRows)
{
	nRows = 0;

	if(!isOpen())
		return CSV::ErrorCode::FILE_NOT_OPEN;

	mCSVFile.flush();
	std::istream::streampos pos = mCSVFile.tellg();
	rewind();

	CSVString l;
	CSV::ErrorCode rc;
	while((rc = readLine(l)) == CSV::ErrorCode::CSV_OK)
		nRows++;

	mCSVFile.seekg(pos);
	if(rc != CSV::ErrorCode::CSV_EOF)
		return rc;

	return CSV::ErrorCode::CSV_OK;
}

CSV::ErrorCode CSV::readLine(CSVString &oLine)
{
	oLine.clear();

	if(!isOpen())
		return CSV::ErrorCode::FILE_NOT_OPEN;

	if(mCSVFile.eof())
		return CSV::ErrorCode::CSV_EOF;

    while(1)
    {
        getline(mCSVFile, oLine);
        int n = oLine.length();
        if(n)
        {
            if(oLine[n-1] == '\r')
                oLine = oLine.substr(0, n-1);

            break;
        }
        else
        {
            if(mCSVFile.eof())
                return CSV::ErrorCode::CSV_EOF;
        }
        if(mCSVFile.bad())
            return CSV::ErrorCode::FILE_STREAM_ERROR;
    }

	return CSV::ErrorCode::CSV_OK;
}

CSV::ErrorCode CSV::readColumns(bool bAutodetectTypes)
{
	if(!hasHeader())
		return CSV::ErrorCode::NO_HEADER;

	if(!isOpen())
		return CSV::ErrorCode::FILE_NOT_OPEN;

	CSVString line;
	rewind(true);
	CSV::ErrorCode rc;
	if((rc = readLine(line)) < 1)
		return rc;

	if(line.length() == 0)
		return CSV::ErrorCode::NO_COLUMNS_FOUND;

	std::vector<CSVString>columns;
	CSVString column;
	int n;
	int col_pos = 0;
	while((n = parseHeader(line, column)) > 0)
	{
		// If the first column starts with a hash we assume it is the
		// comment marker and remove it. If it is only a hash we keep it.
		if(mColumns.size() == 0 && column.size() > 1 && column[0] == '#')
			column = column.substr(1);

		CSVColumn *col = new CSVColumn();
		col->setName(column);
		col->setPosition(col_pos++);
		addColumn(col);

		line = line.substr(n);
	}

	if(rc < 0)
		return CSV::ErrorCode::COLUMN_FIELD_ERROR;

	std::istream::streampos pos = mCSVFile.tellg();
	if(pos == -1)
		return CSV::ErrorCode::CSV_OK;		// Wwe can not determine the datatypes.

	// Now that we read the header, we try to read the first N lines as a sample
	// and try to figure out the datatype.
	std::vector<CSVString> samples;
	for(unsigned int i = 0; i < getSampleRowCount(); i++)
	{
		// Premature end of file if it has less then N sample rows (no error).
		if(readLine(line) < 1)
			break;

		if(line.length() > 0)
			samples.push_back(line);
	}

	// Reset the file back to the first record.
	rewind();

	if(!bAutodetectTypes)
		return CSV::ErrorCode::CSV_OK;

	for(CSVString &sample : samples)
	{
		std::vector<CSVString> row;

		// We still return OK here, because the header was
		// already successfully read. If there is a data error,
		// the user has to deal with it when he actually tries to
		// access the data.
		if(parseRow(sample, row) != CSV::ErrorCode::CSV_OK)
			return CSV::ErrorCode::CSV_OK;

		unsigned int complete = 0;
		std::size_t i = -1;
		std::size_t n = row.size();
		for(CSVColumn *&col : mColumns)
		{
			i++;
			if(col == NULL || col->getType() != spt::db::DataType::type_unknown)
			{
				complete++;
				continue;
			}

			if(i >= n)
				break;

			col->setType(guessType(row[i]));
			//std::cout << "[" << row[i] << "] " << std::endl;
		}

		// If every column has a type, we can exit the loop early.
		if(complete >= mColumns.size())
			break;
	}

	return CSV::ErrorCode::CSV_OK;
}

/**
 * Parsing a row results in the same behaviour as MS Excel would deliver.
 *
 * Example:
 * HEADER: |COLUMN1;COLUMN2;COLUMN3|
 * INPUT:  |"Line1";   "Test"; test"value"|
 * RESULT: [Line1] [   "Test"] [ test"value"]
 *
 * INPUT:  | test"value"dummy|
 * RESULT: [ test"value"dummy] [] []
 *
 * INPUT:  | blabla;"Value includes ;"; and so on|
 * RESULT: [ blabla] [Value includes ;] [ and so on]
 *
 * INPUT:  |"new value"   ;val2;"test" 1234|
 * RESULT: [new value   ] [val2] [test 1234]
 */
CSV::ErrorCode CSV::parseRow(CSVString &oLine, std::vector<CSVString> &oRow)
{
	CSVString val;
	std::vector<CSVColumn *>::size_type columns = mColumns.size();

	int n;
	while((n = parseField(oLine, val)) > 0)
	{
		oRow.push_back(val);
		oLine = oLine.substr(n);
	}

	if(n == -1)
		return CSV::ErrorCode::DATA_FIELD_ERROR;

	return CSV::ErrorCode::CSV_OK;
}

int CSV::parseHeader(CSVString const &oLine, CSVString &oField)
{
	int n = 0;
	std::locale loc;
	CSVChar sep = getSeparator();
	oField = "";

	// If no separator defined and autodetect is disabled we are done and return an error
	if(sep == 0 && !isAutoSeparator())
		return -1;

	bool quote_mode = false;
	for(CSVChar const &c : oLine)
	{
		n++;
		if(sep && c == sep)
			break;

		if(c == '"')
		{
			if(quote_mode == false)
			{
				if(oField.length() == 0)
					quote_mode = true;
				else
					return -1;
			}
			else
				quote_mode = false;

			continue;
		}

		// If the columns are encased in quotes, then we take all characters regardless.
		// Otherwise we only allo certain characters.
		if(quote_mode == true || (quote_mode == false && (c == '#' || c == '_' || c == '-' || std::isalnum(c, loc))))
			oField += c;
		else
		{
			if(sep == 0)
			{
				setSeparator(c);
				break;
			}
			else if(sep != c)
				return -1;
		}
	}

	return n;
}

int CSV::parseField(CSVString const &oLine, CSVString &oField)
{
	CSVChar sep = getSeparator();
	int i = 0;
	int start = 0;
	int end = 0;
	int len = oLine.length();
	CSVChar quote_char = 0;
	CSVChar c;
	oField.clear();

	if(sep == 0)
		return -1;

	// We are done if the line is empty.
	if(len == 0)
		return 0;

	c = oLine[0];
	if(c == sep)	// empty field, so we are done
		return 1;

	// If the line has only one character and it is not the separator then this is the value
	// and will be returned
	if(len == 1)
	{
		oField += c;
		return 1;
	}

	// From here on we may have valid fields, either with data, or encased in quotes
	if(c == '"' || c == '\'')
	{
		quote_char = c;
		i++;
	}

	start = i;
	while(i < len)
	{
		int extra = 1;  // Character after the separator
		int pos = oLine.find(sep, i);
		if(pos == -1)
		{
			pos = len;
			extra = 0;
		}

		i = pos+extra;
		if(quote_char)
		{
			if(oLine[pos-1] == quote_char)
			{
				end = pos-1;
				break;
			}
		}
		else
		{
			end = pos;
			break;
		}
	}

	// Missformed field data.
	if(!end)
		return -1;

	oField = oLine.substr(start, end-start);

	return i;
}

CSV::ErrorCode CSV::read(std::vector<CSVString> &oRow, std::vector<bool> &oNull)
{
	oRow.clear();
	oNull.clear();

	CSVString line;

	CSV::ErrorCode rc;
	if((rc = readLine(line)) < 1)
		return rc;

	if(line.length() == 0 && mCSVFile.eof())
		return CSV::ErrorCode::CSV_EOF;

	if((rc = parseRow(line, oRow)) != CSV::ErrorCode::CSV_OK)
	{
		oRow.clear();
		oNull.clear();
		return rc;
	}

	if(oRow.size() > mColumns.size())
		return CSV::ErrorCode::DATA_COLUMN_MISSMATCH;

	for(std::size_t i = 0; i < mColumns.size(); i++)
	{
		if(i >= oRow.size())		// fill up empty values with null.
		{
			CSVString val;
			oRow.push_back(val);
			oNull.push_back(true);
		}
		else if(i >= oNull.size())
			oNull.push_back(false);

		mColumns[i]->setNull(oNull[i]);
	}

	return CSV::ErrorCode::CSV_OK;
}

CSV::ErrorCode CSV::read(void)
{
	std::vector<CSVString> row;
	std::vector<bool> null;
	CSV::ErrorCode rc;

	if((rc = read(row, null)) !=CSV::ErrorCode::CSV_OK)
		return rc;

	std::size_t i = -1;
	for(CSVColumn *&c : mColumns)
	{
		i++;
		c->setNull(null[i]);
		c->setValue(row[i]);
	}

	return CSV::ErrorCode::CSV_OK;
}

CSV::ErrorCode CSV::writeHeader(void)
{
	if(!isOpen())
		return CSV::ErrorCode::FILE_NOT_OPEN;

	if(!hasHeader() || mColumns.size() == 0)
		return CSV::ErrorCode::NO_HEADER;

    CSVString h;
    CSVChar sep = getSeparator();
    CSVChar closer;
    CSVChar opener = getBracket(closer);

	for(CSVColumn *&c : mColumns)
	{
		if(opener)
			h += opener;

		h += c->getName();
		if(closer)
			h += closer;
		if(sep)
			h += sep;
	}

	h = h.substr(0, h.length()-1);
	h += "\n";

	mCSVFile.seekg(0);
	mCSVFile.clear();
	mCSVFile << h;
	if(mCSVFile.fail())
		return CSV::ErrorCode::FILE_STREAM_ERROR;

	return CSV::ErrorCode::CSV_OK;
}

CSV::ErrorCode CSV::write(std::vector<CSVString> const &oRow, std::vector<bool> &oNull)
{
	if(!isOpen())
		return CSV::ErrorCode::FILE_NOT_OPEN;

	CSVString s;
	int n = oRow.size();
	int i = -1;
	CSVChar sep = getSeparator();
	CSVChar closer;
	CSVChar opener = getBracket(closer);
	for(CSVColumn *&col : mColumns)
	{
		if(!col)
			continue;

		i++;
		if(i < n && !oNull[i])
		{
			if(opener)
				s += opener;

			s += oRow[i];

			if(closer)
				s += closer;
		}

		if(sep)
			s += sep;
	}

	s = s.substr(0, s.length()-1);
	s += "\n";
	//mCSVFile.write(s.c_str(), s.length());
	mCSVFile << s;
	if(mCSVFile.fail())
		return CSV::ErrorCode::FILE_STREAM_ERROR;

	return CSV::ErrorCode::CSV_OK;
}
