/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <ctime>

#include "support/logging/logging.h"

namespace spt
{
namespace logging
{

LoggingItem::LoggingItem(void)
{
	mLine = 0;
	mType = LogType::LOG_UNDEFINED;
	mTimestamp = 0;
}

LoggingItem::LoggingItem(LoggingItem const &oSource)
{
	copy(oSource);
}

LoggingItem::LoggingItem(spt::string::string_t const &oMessage, spt::string::string_t const &oUnit, LogType nType, unsigned int nTimestamp, spt::string::string_t const &oFile, int nLine, spt::string::string_t const &oMethod)
{
	setMessage(oMessage);
	setUnit(oUnit);
	setType(nType);
	setTimestamp(nTimestamp);
	setFile(oFile);
	setMethod(oMethod);
	setLine(nLine);
}

LoggingItem::~LoggingItem(void)
{
}

void LoggingItem::copy(LoggingItem const &oSource)
{
	mLine = oSource.mLine;
	mType = oSource.mType;
	mTimestamp = oSource.mTimestamp;
	mFile = oSource.mFile;
	mMethod = oSource.mMethod;
	mMessage = oSource.mMessage;
	mUnit = oSource.mUnit;
}

LoggingItem &LoggingItem::operator=(LoggingItem const &oSource)
{
	copy(oSource);

	return *this;
}

void LoggingItem::setTimestamp(size_t nTimestamp)
{
	if(nTimestamp == 0)
	{
		nTimestamp = time(0);
		nTimestamp *= 1000;
	}

	mTimestamp = nTimestamp;
}

spt::string::string_t LoggingItem::getTypeText(void) const
{
	LogType t = getType();

	if(t == LOG_ERROR)
		return "ERROR";

	if(t == LOG_WARN)
		return "WARNING";

	if(t == LOG_INFO)
		return "INFO";

	if(t == LOG_DEBUG)
		return "DEBUG";

	return "UNDEFINED";
}

/***********************************************************************/
// Logging

Logging::Logging(void)
{
}

Logging::~Logging(void)
{
}

void Logging::addEntry(spt::string::string_t const &oMessage, spt::string::string_t const &oUnit, LoggingItem::LogType nType, unsigned int nTimestamp, spt::string::string_t const &oFile, int nLine, spt::string::string_t const &oMethod)
{
    addItem(LoggingItem(oMessage, oUnit, nType, nTimestamp, oFile, nLine, oMethod));
}

// namespace
}
}
