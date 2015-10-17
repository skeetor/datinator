/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef DATABASE_LOGIN_H
#define DATABASE_LOGIN_H

#include "support/unicode/unicode_types.h"
#include "support/support_dll_api.h"
#include "support/patterns/serialize.h"

class SUPPORT_DLL_EXPORT DatabaseLogin : public Serialize<spt::string::string_t>
{
public:
	DatabaseLogin(void);
	DatabaseLogin(DatabaseLogin const *oSource);
	DatabaseLogin(DatabaseLogin const &oSource);
	DatabaseLogin(spt::string::string_t const &oSerialized);
	virtual ~DatabaseLogin(void);

	virtual void clear(void);

	virtual bool equals(DatabaseLogin const &oSource) const;
	virtual bool operator==(DatabaseLogin const &oSource) const;
	virtual bool operator!=(DatabaseLogin const &oSource) const;

	virtual DatabaseLogin &operator=(DatabaseLogin const &oSource);
	virtual void copy(DatabaseLogin const &oSource);
	virtual void copy(DatabaseLogin const *oSource);

	virtual spt::string::string_t getUser(void) const;
	virtual void setUser(spt::string::string_t const &oUser);

	virtual spt::string::string_t getPassword(void) const;
	virtual void setPassword(spt::string::string_t const &oPassword);

	virtual spt::string::string_t getClearPassword(void) const;
	virtual void setClearPassword(spt::string::string_t const &oPassword);

	virtual spt::string::string_t getDatabase(void) const;
	virtual void setDatabase(spt::string::string_t const &oDatabase);

	virtual bool requiresSaveHost(void) const;
	virtual void saveHost(bool oSaveHost = true);

	/**
	 * getHost() returns the full hostname in the form
	 * hostname:port
	 */
	virtual spt::string::string_t getHost(void) const;
	virtual void setHost(spt::string::string_t const &oHost);

	/**
	 * getHostname() returns only the hostname
	 */
	virtual spt::string::string_t getHostname(void) const;
	virtual void setHostname(spt::string::string_t const &oHostname);
	virtual int getPort(void) const;
	virtual void setPort(int oHost);

public: // @Serialize<T, S>
	virtual spt::string::string_t serialize(void);
	virtual bool deserialize(spt::string::string_t const &oList);

protected:
	void splitHost(void);

	virtual spt::string::string_t encrypt(spt::string::string_t const &oPassword) const;
	virtual spt::string::string_t decrypt(void) const;

private:
	bool mSaveHost;
	int mPort;
	spt::string::string_t mHost;
	spt::string::string_t mUser;
	spt::string::string_t mPassword;
	spt::string::string_t mDatabase;

};

#endif // DATABASE_LOGIN_H
