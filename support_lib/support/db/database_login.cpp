/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "support/db/database_login.h"
#include "support/helper/string.h"

#define ENCRYPT_VALUE 0x7b

DatabaseLogin::DatabaseLogin(void)
{
	clear();
}

DatabaseLogin::DatabaseLogin(DatabaseLogin const *oSource)
{
	copy(oSource);
}

DatabaseLogin::DatabaseLogin(DatabaseLogin const &oSource)
{
	copy(oSource);
}

DatabaseLogin::DatabaseLogin(supportlib::string::string_t const &oSerialized)
{
	if(!deserialize(oSerialized))
		clear();
}

DatabaseLogin::~DatabaseLogin()
{
}

void DatabaseLogin::clear(void)
{
	setUser("");
	setPassword("");
	setDatabase("");
	setHost("");
	setPort(-1);
	saveHost(false);
}

DatabaseLogin &DatabaseLogin::operator=(DatabaseLogin const &oSource)
{
	copy(oSource);
	return *this;
}

void DatabaseLogin::copy(DatabaseLogin const *oSource)
{
	if(oSource == NULL)
		clear();
	else
		copy(*oSource);
}

void DatabaseLogin::copy(DatabaseLogin const &oSource)
{
    setUser(oSource.getUser());
    setPassword(oSource.getPassword());
	setDatabase(oSource.getDatabase());
	setHost(oSource.getHost());
	setPort(oSource.getPort());
	saveHost(oSource.requiresSaveHost());
}

supportlib::string::string_t DatabaseLogin::getUser(void) const
{
	return mUser;
}

void DatabaseLogin::setUser(supportlib::string::string_t const &oUser)
{
	mUser = oUser;
}

supportlib::string::string_t DatabaseLogin::encrypt(supportlib::string::string_t const &oPassword) const
{
	int bsz = (oPassword.length()+3)*2;
	char *b = new char[bsz];
	memset(b, 0, bsz);

	int i = 0;
	unsigned char byte = oPassword.length();	// Password should not be longer than 255 bytes, but that should suffice.
	char s[8];
	sprintf(s, "%02X", byte ^ ENCRYPT_VALUE);
	b[i++] = s[0];
	b[i++] = s[1];
	for(char const &c : oPassword)
	{
		byte = c ^ ENCRYPT_VALUE;
		sprintf(s, "%02X", byte);
		b[i++] = s[0];
		b[i++] = s[1];
	}

	supportlib::string::string_t str = b;
	memset(b, 0, bsz);
	delete [] b;

	return str;
}

supportlib::string::string_t DatabaseLogin::decrypt(void) const
{
	// The first two characters are the length. If there is no more
	// then the password is empty.
	int len = mPassword.length();

	supportlib::string::string_t str;
	unsigned char byte = 0;
	unsigned char c;
	int i = 1;

	while(i < len)
	{
		i++;
		if(i >= len)
			break;

		c = mPassword[i];
		if(c >= '0' && c <= '9')
			c -= '0';
		else
		{
			if(c >= 'A' && c <= 'F')
				c -= 'A';
			else if(c >= 'a' && c <= 'f')
				c -= 'a';
			else
				break;		// invalid character

			c += 10;
		}

		if(i & 1)
		{
			byte += c;
			str += byte ^ ENCRYPT_VALUE;
		}
		else
			byte = c * 0x10;
	}

	return str;
}

supportlib::string::string_t DatabaseLogin::getPassword(void) const
{
	return mPassword;
}

void DatabaseLogin::setPassword(supportlib::string::string_t const &oPassword)
{
	mPassword = oPassword;
}

supportlib::string::string_t DatabaseLogin::getDatabase(void) const
{
	return mDatabase;
}

supportlib::string::string_t DatabaseLogin::getClearPassword(void) const
{
	return decrypt();
}

void DatabaseLogin::setClearPassword(supportlib::string::string_t const &oPassword)
{
	setPassword(encrypt(oPassword));
}

void DatabaseLogin::setDatabase(supportlib::string::string_t const &oDatabase)
{
	mDatabase = oDatabase;
}

supportlib::string::string_t DatabaseLogin::getHostname(void) const
{
	return mHost;
}

void DatabaseLogin::setHostname(supportlib::string::string_t const &oHost)
{
	mHost = oHost;
}

supportlib::string::string_t DatabaseLogin::getHost(void) const
{
	supportlib::string::string_t h = mHost;

	int port = getPort();
	if(port != -1)
	{
		supportlib::string::stringstream_t ss;
		supportlib::string::string_t s;
		h += ':';
		ss << port;
		ss >> s;
		h += s;
	}

	return h;
}

void DatabaseLogin::setHost(supportlib::string::string_t const &oHost)
{
	mHost = oHost;
	splitHost();
}

void DatabaseLogin::splitHost(void)
{
}

int DatabaseLogin::getPort(void) const
{
	return mPort;
}

void DatabaseLogin::setPort(int nPort)
{
	mPort = nPort;
}

bool DatabaseLogin::requiresSaveHost(void) const
{
	return mSaveHost;
}

void DatabaseLogin::saveHost(bool bSaveHost)
{
	mSaveHost = bSaveHost;
}

bool DatabaseLogin::equals(DatabaseLogin const &oSource) const
{
	if(oSource.getHost() != getHost())
		return false;

	if(oSource.getDatabase() != getDatabase())
		return false;

	if(oSource.getUser() != getUser())
		return false;

	return true;
}

bool DatabaseLogin::operator==(DatabaseLogin const &oSource) const
{
	return equals(oSource);
}	int i = 1;


bool DatabaseLogin::operator!=(DatabaseLogin const &oSource) const
{
	return !equals(oSource);
}

supportlib::string::string_t DatabaseLogin::serialize(void)
{
	supportlib::string::string_t s;

	s += getUser()+';';
	s += getPassword()+';';
	s += getDatabase()+';';
	if(requiresSaveHost())
	{
		s += "T;";
		s += getHost()+";";
	}
	else
		s += "F;;";		// Host is empty

	return s;
}

bool DatabaseLogin::deserialize(supportlib::string::string_t const &oSerialized)
{
	clear();

	std::vector<supportlib::string::string_t> values = supportlib::string::split(oSerialized, ';');
	size_t i = 0;
	if(values.size() > i)
		setUser(values[i]);

	i++;
	if(values.size() > i)
		setPassword(values[i]);

	i++;
	if(values.size() > i)
		setDatabase(values[i]);

	i++;
	if(values.size() > i)
	{
		bool hs = false;
		supportlib::string::string_t s = values[i];
		if(s.length() > 0)
		{
			if(s[0] == 'T')
				hs = true;
		}

		saveHost(hs);
		if(hs == true)
		{
			i++;
			if(values.size() > i)
				setHost(values[i]);
		}
	}

	return true;
}
