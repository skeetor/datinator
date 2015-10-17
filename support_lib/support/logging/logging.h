/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef _SUPPORT_LOGGING_H
#define _SUPPORT_LOGGING_H

#include "support/unicode/unicode_types.h"
#include "support/support_dll_api.h"
#include <vector>

namespace spt
{
namespace logging
{
	class SUPPORT_DLL_EXPORT LoggingItem
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
		LoggingItem(spt::string::string_t const &oMessage, spt::string::string_t const &oUnit = "", LogType nType = LOG_UNDEFINED, unsigned int nTimestamp = 0, spt::string::string_t const &oFile = "", int nLine = 0, spt::string::string_t const &oMethod = "");
		virtual ~LoggingItem(void);

		virtual void copy(LoggingItem const &oSource);
		virtual LoggingItem &operator=(LoggingItem const &oSource);

		inline int getLine(void) const { return mLine; };
		inline void setLine(int nLine) { mLine = nLine; };

		spt::string::string_t getTypeText(void) const;
		inline LogType getType(void) const { return mType; };
		inline void setType(LogType nType) { mType = nType; };

		inline size_t getTimestamp(void) const { return mTimestamp; };
		void setTimestamp(size_t nTimestamp = 0);		// set the current systemtime as the timestamp if == 0

		inline spt::string::string_t getFile(void) const { return mFile; };
		inline void setFile(spt::string::string_t const &oFile) { mFile = oFile; };

		inline spt::string::string_t getMethod(void) const { return mMethod; };
		inline void setMethod(spt::string::string_t const &oMethod) { mMethod = oMethod; };

		inline spt::string::string_t getMessage(void) const { return mMessage; };
		inline void setMessage(spt::string::string_t const &oMessage) { mMessage = oMessage; };

		inline spt::string::string_t getUnit(void) const { return mUnit; };
		inline void setUnit(spt::string::string_t const &oUnit) { mUnit = oUnit; };

	private:
		LogType mType;
		int mLine;
		size_t mTimestamp;
		spt::string::string_t mFile;
		spt::string::string_t mMethod;
		spt::string::string_t mUnit;
		spt::string::string_t mMessage;
	};

	class SUPPORT_DLL_EXPORT Logging
	{
	public:
		Logging(void);
		virtual ~Logging(void);

		virtual void addEntry(spt::string::string_t const &oMessage, spt::string::string_t const &oUnit = "", LoggingItem::LogType nType = LoggingItem::LogType::LOG_UNDEFINED, unsigned int nTimestamp = 0, spt::string::string_t const &oFile = "", int nLine = 0, spt::string::string_t const &oMethod = "");

		inline void addItem(LoggingItem const &oItem) { mItems.push_back(oItem); } ;
		inline std::vector<LoggingItem> getItems(void) const { return mItems; };

		inline void clear(void) { mItems.clear(); };
		inline int count(void) { return mItems.size(); };

	private:
		std::vector<LoggingItem> mItems;
	};

}
}

#endif // _SUPPORT_LOGGING_H
