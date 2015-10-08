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

#ifndef CSV_H
#define CSV_H

#include <cstring>
#include <string>
#include <vector>
#include <fstream>

#include <support/support_dll_api.h>
#include "support/db/dbcolumn.h"

#define DEFAULT_SAMPLE_ROWS  5

typedef supportlib::string::char_t CSVChar;
typedef supportlib::string::string_t CSVString;
typedef supportlib::db::DBColumn<CSVString, CSVString> CSVColumn;

class SUPPORT_DLL_EXPORT CSV
{
public:
	typedef enum
	{
		CSV_EOF					= 	0,		// These two shouldn't be changed.
		CSV_OK					= 	1,
		FILE_NOT_OPEN			= -1,
		FILE_ALREADY_OPEN		= -2,
		FILE_BAD_STREAM			= -3,
		FILE_STREAM_ERROR		= -4,
		READ_PARSE_ROW_ERROR	= -5,
		NO_HEADER				= -6,
		NO_COLUMNS_FOUND		= -7,
		COLUMN_FIELD_ERROR		= -8,
		DATA_FIELD_ERROR		= -9,
		DATA_COLUMN_MISSMATCH	= -10,		// This is returned only if there are more columns in the row than in the header.
		NOT_SUPPORTED			= -11,
	} ErrorCode;

	typedef struct
	{
		unsigned int APPEND		: 1;		// Append to file, error if only READ is specified.
		unsigned int AT_END		: 1;		// Seek to end of the file so output starts there.
		unsigned int BINARY		: 1;
		unsigned int READ		: 1;
		unsigned int WRITE 		: 1;
		unsigned int TRUNCATE	: 1;
		unsigned int CREATE		: 1;		// If the file doesn't exist, then create it.
	} _s_Openmode;

	class Openmode
	{
	public:
		Openmode(void) { memset(&flags, 0, sizeof(_s_Openmode)); }
		Openmode(Openmode const &oSource) { memcpy(&flags, &oSource.flags, sizeof(_s_Openmode)); }
		inline Openmode &operator=(Openmode const &oSource) { memcpy(&flags, &oSource.flags, sizeof(_s_Openmode)); return *this; }

	public:
		_s_Openmode flags;
	};

public:
	CSV();
	virtual ~CSV();

	const char *toErrorText(CSV::ErrorCode nCode);

	/**
	 * setHeader() must be used if a CSV file has no header record. By default it is assumed
	 * that the first line of a CSV file contains a record with the columnnames. A columnname
	 * may contain letters, numbers and '-' or '_'. The first non matching character is assumed
	 * to be the separator. If a CSV file has no such record and starts immediately with data
	 * the Header must be set to false. Obviously in this case it is not possible to determine
	 * the column names and numbers, so the user must provide this if needed. This must be
	 * called before open() is used.
	 */
	void setHeader(bool bHeader);
	bool hasHeader(void) const;

	void setSeparator(CSVChar oSeparator);
	CSVChar getSeparator(void) const;

	/**
	 * The brackets define how an indvidual field is enclosed. Open is the front
	 * and close is the closing bracket. Note that the term "bracket" doesn't neccessarily
	 * mean real brackets, it can be any characters.
	 * Default is -"- for both.
	 */
	void setBracket(CSVChar oOpen = '"', CSVChar oClose = '"');
	CSVChar getBracket(CSVChar &oClose) const;

	/**
	 * If the headers are read from a file, the code will try to determine
	 * the datatypes algorithmically. This is of course just a guess and can
	 * produce wrong numbers. This is done by reading the first N records of
	 * the file and look at the values. If N is set to 0 this feature is disabled.
	 *
	 * After reading the columns, the user can of course change the datatypes
	 * always manually.
	 */
	void setSampleRowCount(unsigned int oSampleNumber);
	unsigned int getSampleRowCount(void) const;

	/**
	 * If a new file is opened and the separator is set to 0, then it
	 * will be automatically determined. The first spearator character
	 * from the header will be used throughout the file then.
	 * If this happens and the file is closed, the separator is automatically
	 * reset. Otherwise the user specified separator is kept.
	 */
	bool isAutoSeparator(void) const;

	CSVString const getFilename(void) const;
	void setFilename(CSVString const &oFilename);

	/**
	 * This will open the current file. If successful, true is returned.
	 * If the file is already open and open() is called again, it will
	 * simply return and nothing happens, even if a new filename has been
	 * set. To open a new file, close() must be called first, to ensure that
	 * the stream is closed.
	 */
	virtual CSV::ErrorCode open(Openmode &oOpenMode);
	virtual void close(void);
	virtual bool isOpen(void);

	virtual CSV::ErrorCode count(unsigned int &nRows);

	/**
	 * Reads the values of the current record. The values are stored in
	 * the column values. If < 0 is returned, an error occured. If no
	 * records are available anymore the return value is 0, otherwise 1.
	 * If the error is from readLine(), then that errorcode is returned.
	 */
	virtual CSV::ErrorCode read(void);

	/**
	 * same as read(void), but it returns the values in the specified
	 * vector, instead of the columns.
	 */
	virtual CSV::ErrorCode read(std::vector<CSVString> &oRow, std::vector<bool> &oNull);

	/**
	 * The elements in the row, must match the number of elements in the columns.
	 */
	virtual CSV::ErrorCode write(std::vector<CSVString> const &oRow, std::vector<bool> &oNull);

	/**
	 * rewind() will cause the stream to be reverted to the first record
	 * if bAbsolute == false, thus advancing over the header line, otherwise
	 * it will be rewound to 0.
	 */
	virtual CSV::ErrorCode rewind(bool bAbsolute = false);

	virtual void setColumns(std::vector<CSVColumn *> const &oColumns);
	virtual std::vector<CSVColumn *> getColumns(void);
	virtual void addColumn(CSVColumn *oColumn, int nPosition = -1);
	virtual void clearColumns(void);

	CSV::ErrorCode writeHeader(void);

protected:
	/**
	 * Reads the first line of the CSV file. It is assumed that this
	 * contains the names of the columns. If hasHeader() returns false
	 * this function will do nothing. If the parameter bAutodetecTypes is
	 * set to true, then the first few lines will be read and the types
	 * of the columns will be determined. This is only a guess though, so
	 * depending on the values, this may be incorrect.
	 *
	 * Attention: readColumns() will rewind to the beginning of the file
	 * so that a read afterwards will read the next record.
	 *
	 * A special treatment is given the character '#' if used as the first
	 * character in the file. If the hash appears as the first character
	 * and immediately afterwards follows a separator, then the hash is
	 * taken as column name. If the it is followed by a columnname character
	 * it will be discarded.
	 * Example:
	 * #_COLUMN1;_COLUMN2
	 * #0_COLUMN1;1_COLUMN2
	 * #COLUMN1;COLUMN2       The hash is discarded
	 *
	 * #;COLUMN1;COLUMN2      The hash is kept as a separate column.
	 */
	virtual CSV::ErrorCode readColumns(bool bAutodetectTypes = true);

	/**
	 * readLine() will read a full line into the string. -1 is returned
	 * if the stream is not open. If an I/O error was reported -1 is returned.
	 * -2 is returned when a read error occurs.
	 * 0 is EOF.
	 * If a partial line is read and EOF is hit as a result, then true will be
	 * returned. Only on the next try it will already be at EOF and then 0
	 * is returned.
	 * A positive value means a successfull read.
	 */
	virtual CSV::ErrorCode readLine(CSVString &oLine);

	/**
	 * parse a row into individual columns. If a column definition exists
	 * and the line provides lesser columns then specified by the header,
	 * it will be filled to match the header. If there are more columns
	 * then defined by the header, the values are still read adn the caller
	 * has to decide what to do with the extra columns.
	 */
	virtual CSV::ErrorCode parseRow(CSVString &oLine, std::vector<CSVString> &oRow);

	/**
	 * Read a single field from the input line. The total length, up to the separator, of the
	 * field is returned if successfull. If an error occurs -1 is returned.
	 * If the separator is not set (getSeparator() == 0), it is assumed that it is the first
	 * special character encountered. A special character is any that is not ["_'] or ["-'] or ["#']
	 * isDigit() or isCharacter(). In other words, a header column name may only
	 * consist of alphabetic characters, digits and '#', '_' or '-'. No umlauts or other special
	 * characters.
	 *
	 * A normal field must be separated by the separation character. If the first character of the field is
	 * a ' or a " character it is assumed that the whole field is encased in the respective quotes. In this
	 * case the end of the field must be marked by the quoting character immediately followed by the separator.
	 * The last separator at the end of the line can be ommited, but in this case no whitespaces may follow.
	 * If the field consists only of a single quote, then this is returned as the value. i.E.
	 * "val";";
	 * or
	 * "val";"
	 *
	 * will both be returned as 'val', '"'.
	 */
	virtual int parseField(CSVString const &oLine, CSVString &oField);
	virtual int parseHeader(CSVString const &oLine, CSVString &oField);

	/**
	 * Returns true if a character can be a separator, otherwise it returns false,
	 * which means that the char is part of a valid columnname.
	 *
	 * A column name contains "A-Z", "a-z", "0-9" and '_', anything else is a
	 * separator. Note that german umlauts and other special characters are NOT
	 * part of a valid column name.
	 */
	virtual bool isSeparator(CSVChar oChar);

private:
	int mSampleRows;
	bool mHeader;
	bool mAutoSeparator;
	CSVChar *mStructAdress;			// Offset used when multiple structures are loaded.
	CSVChar mSeparator;
	CSVChar mOpener;				// Bracket
	CSVChar mCloser;				// Bracket
	CSVString mFilename;
	std::vector<CSVColumn *> mColumns;
	std::fstream mCSVFile;
};

#endif // CSV_H
