/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QDateTime>

#include "support_qt/logging/logging.h"

namespace supportlib
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

LoggingItem::LoggingItem(QString const &oMessage, QString const &oUnit, LogType nType, unsigned int nTimestamp, QString const &oFile, int nLine, QString const &oMethod)
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

void LoggingItem::setTimestamp(qint64 nTimestamp)
{
	if(nTimestamp == 0)
		nTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

	mTimestamp = nTimestamp;
}

QString LoggingItem::getTypeText(void) const
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

void Logging::addEntry(QString const &oMessage, QString const &oUnit, LoggingItem::LogType nType, unsigned int nTimestamp, QString const &oFile, int nLine, QString const &oMethod)
{
    addItem(LoggingItem(oMessage, oUnit, nType, nTimestamp, oFile, nLine, oMethod));
}

// namespace
}
}
