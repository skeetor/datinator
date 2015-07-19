/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef LOGGING_H
#define LOGGING_H

#include <QtCore/QString>
#include <QtCore/QList>

#include "support_qt/support_qt_dll_api.h"

namespace supportlib
{
namespace logging
{
	class SUPPORT_QT_DLL_EXPORT LoggingItem
	{
	public:
		typedef enum
		{
			LOG_ERROR,
			LOG_WARN,
			LOG_INFO,
			LOG_DEBUG,
			LOG_UNDEFINED
		} LogType;

	public:
		LoggingItem(void);
		LoggingItem(LoggingItem const &oSource);
		LoggingItem(QString const &oMessage, QString const &oUnit = "", LogType nType = LOG_UNDEFINED, unsigned int nTimestamp = 0, QString const &oFile = "", int nLine = 0, QString const &oMethod = "");
		virtual ~LoggingItem(void);

		virtual void copy(LoggingItem const &oSource);
		virtual LoggingItem &operator=(LoggingItem const &oSource);

		inline int getLine(void) const { return mLine; };
		inline void setLine(int nLine) { mLine = nLine; };

		QString getTypeText(void) const;
		inline LogType getType(void) const { return mType; };
		inline void setType(LogType nType) { mType = nType; };

		inline qint64 getTimestamp(void) const { return mTimestamp; };
		void setTimestamp(qint64 nTimestamp = 0);		// set the current systemtime as the timestamp if == 0

		inline QString getFile(void) const { return mFile; };
		inline void setFile(QString const &oFile) { mFile = oFile; };

		inline QString getMethod(void) const { return mMethod; };
		inline void setMethod(QString const &oMethod) { mMethod = oMethod; };

		inline QString getMessage(void) const { return mMessage; };
		inline void setMessage(QString const &oMessage) { mMessage = oMessage; };

		inline QString getUnit(void) const { return mUnit; };
		inline void setUnit(QString const &oUnit) { mUnit = oUnit; };

	private:
		LogType mType;
		int mLine;
		qint64 mTimestamp;
		QString mFile;
		QString mMethod;
		QString mUnit;
		QString mMessage;
	};

	class SUPPORT_QT_DLL_EXPORT Logging
	{
	public:
		Logging(void);
		virtual ~Logging(void);

		virtual void addEntry(QString const &oMessage, QString const &oUnit = "", LoggingItem::LogType nType = LoggingItem::LogType::LOG_UNDEFINED, unsigned int nTimestamp = 0, QString const &oFile = "", int nLine = 0, QString const &oMethod = "");

		inline void addItem(LoggingItem const &oItem) { mItems.append(oItem); } ;
		inline QList<LoggingItem> getItems(void) const { return mItems; };

		inline void clear(void) { mItems.clear(); };
		inline int count(void) { return mItems.count(); };

	private:
		QList<LoggingItem> mItems;
	};

}
}

#endif // LOGGING_H
