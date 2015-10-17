/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef DBCOLUMN_H
#define DBCOLUMN_H

#include <string>
#include "support/support_dll_api.h"
#include "support/unicode/unicode_types.h"
#include "support/db/column_types.h"
#include "support/db/type_helper.h"

namespace spt
{
namespace db
{

template <typename T, typename V>
class DBColumn
{
public:
	DBColumn(void)
	{
		init();
	}

	DBColumn(T const &oName, DataType oType, T const &oNativeType, bool bNullable, int nPosition, int nSize, int nPrecision = 0)
	{
		init();

		setName(oName);
		setType(oType);
		setNativeType(oNativeType);
		setPosition(nPosition);
		setSize(nSize);
		setPrecision(nPrecision);
		setNullable(bNullable);
	}

	DBColumn(DBColumn<T, V> const *oSource)
	{
		init();
		copy(oSource);
	}

	DBColumn(DBColumn<T, V> const &oSource)
	{
		init();
		copy(oSource);
	}

	virtual ~DBColumn(void)
	{
	}

	virtual void reset(void)
	{
		init();
	}

	/**
	 * This will create a new object from the current
	 * column. The new object must be deleted to avoid
	 * memory leaks as it is dynamically allocated.
	 */
	virtual DBColumn<T, V> *duplicate(void)
	{
		return new DBColumn<T, V>(*this);
	}

	virtual T getName(void) const
	{
		 return mName;
	}

	virtual void setName(T const &oName)
	{
		mName = oName;
	}

	virtual V getValue(void) const
	{
		 return mValue;
	}

	virtual void setValue(V const &oValue)
	{
		mValue = oValue;
	}

	bool isNull(void) const
	{
		 return mNull;
	}

	void setNull(bool bNull)
	{
		mNull = bNull;
	}

	/**
	 * Set this to true if the value allows a NULL value.
	 */
	bool isNullable() const
	{
		 return mNullable;
	}

	void setNullable(bool bNullable)
	{
		mNullable = bNullable;
	}

	virtual DataType getType(void) const
	{
		return mType;
	}

/*	virtual DataType setType(T const &oDataType)
	{
		DataType type = toType(oDataType);
		setType(type);
		return type;
	}
*/
	virtual void setType(DataType oDataType)
	{
		switch(oDataType)
		{
			case type_string:
			case type_integer:
			case type_decimal:
			case type_date:
			case type_datetime:
			case type_datetime_long:
			case type_time:
			case type_time_long:
			case type_ip:
			break;

			default:
				oDataType = type_unknown;
		}

		mType = oDataType;
	}

	T getNativeType() const
	{
		 return mNativeType;
	}

	void setNativeType(T const &oNativeType)
	{
		mNativeType = oNativeType;
	}

	int getPosition(void) const
	{
		return mPosition;
	}

	void setPosition(int nPosition)
	{
		mPosition = nPosition;
	}

	int getSize(void) const
	{
		return mSize;
	}

	void setSize(int nSize)
	{
		mSize = nSize;
	}

	int getPrecision(void) const
	{
		return mPrecision;
	}

	void setPrecision(int nPrecision)
	{
		mPrecision = nPrecision;
	}

	virtual DBColumn<T, V> &operator=(DBColumn<T, V> const &oSource)
	{
		copy(oSource);
		return *this;
	}

	virtual void copy(DBColumn<T, V> const *oSource)
	{
		if(oSource == NULL)
			reset();
		else
			copy(*oSource);
	}

	virtual void copy(DBColumn<T, V> const &oSource)
	{
		setName(oSource.getName());
		setValue(oSource.getValue());
		setType(oSource.getType());
		setNativeType(oSource.getNativeType());
		setPosition(oSource.getPosition());
		setSize(oSource.getSize());
		setPrecision(oSource.getPrecision());
		setNull(oSource.isNull());
		setNullable(oSource.isNullable());
	}

private:
	void init(void)
	{
		setName("");
		setValue("");
		setPosition(-1);
		setType(DataType::type_unknown);
		setNativeType("");
		setNull(true);
		setNullable(true);
		setSize(0);
		setPrecision(0);
	}

private:
	int mPosition;
	int mSize;
	int mPrecision;
	bool mNull;
	bool mNullable;
	DataType mType;
	T mName;
	V mValue;
	T mNativeType;
};

} // namespace db
} // namespace spt

#endif // DBCOLUMN_H
