/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef SERIALIZE_INTERFACE_INCLUDED_H
#define SERIALIZE_INTERFACE_INCLUDED_H

/**
 * Serialize an object to type T
 */
template <typename T>
class Serialize
{
public:
	Serialize(void) {};
	virtual ~Serialize(void) {};

	virtual T serialize(void) = 0;
	virtual bool deserialize(T const &) = 0;
};

#endif // SERIALIZE_INTERFACE_INCLUDED_H
