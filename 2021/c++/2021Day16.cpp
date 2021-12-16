/// \file 2021Day16.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/16.

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cassert>
#include <memory>
#include <climits>

#include "utilities.hpp"

/// A stream of bits that can be encoded by pushing onto the back, decoded by popping from the front.
using Bits = std::deque<bool>;

/// The number of bits used to store a version.
constexpr unsigned int VERSION_WIDTH = 3U;
/// The number of bits used to store a typeID.
constexpr unsigned int TYPEID_WIDTH = 3U;
/// The largest possible verison value.
constexpr unsigned int MAX_VERSION = 1U << VERSION_WIDTH;
/// The largest possible typeID value.
constexpr unsigned int MAX_TYPEID = 1U << TYPEID_WIDTH;
/// The number of data bits in each chunk of a literal value.
constexpr unsigned int LITERAL_CHUNK_WIDTH = 4U;
/// The number of bits used to store a length of all subpackets combined.
constexpr unsigned int LENGTH_OF_SUBPACKETS_WIDTH = 15U;
/// The number of bits used to store the number of subpackets.
constexpr unsigned int NUMBER_OF_SUBPACKETS_WIDTH = 11U;

/// An operator packet that adds any number of subpackets.
constexpr unsigned int TYPE_SUM = 0U;
/// An operator packet that multiplies any number of subpackets.
constexpr unsigned int TYPE_PRODUCT = 1U;
/// An operator packate that finds the minimum of at least one subpacket.
constexpr unsigned int TYPE_MINIMUM = 2U;
/// An operator packet that finds the maximum of at least one subpacket.
constexpr unsigned int TYPE_MAXIMUM = 3U;
/// A literal packet.
constexpr unsigned int TYPE_LITERAL = 4U;
/// An operator packet that checks whether or not the first of two subpackets is larger.
constexpr unsigned int TYPE_GREATER = 5U;
/// An operator packet that checks whether or not the first of two subpackets is smaller.
constexpr unsigned int TYPE_LESSER = 6U;
/// An operator packet that checks whether or not two subpackets are equal.
constexpr unsigned int TYPE_EQUAL = 7U;

/// \brief Extracts some number of bits from a bit stream and interprets them as an unsigned number.
/// \param[inout] bits The bit stream.
/// \param[in] length The number of bits to extract.
/// \return The interpretation of those bits.
/// \post That many bits have been extracted from the stream.
unsigned int parseNumber (Bits & bits, unsigned int length) {
    unsigned int value = 0U;
    for (unsigned int count {0U}; count < length; ++count) {
        value <<= 1;
        value += bits.front ();
        bits.pop_front ();
    }
    return value;
}


/// \brief A packet received in the Buoyancy Interchange Transmission System.
class Packet {
public:
    /// \brief Constructs a packet.
    /// \param[in] version The version number of the packet, which must be a number 0-8.
    /// \param[in] typeID The type of the packet, which must be a number 0-8.
    /// \return The values have been stored.
    Packet (unsigned int version, unsigned int typeID)
        : m_version (version), m_typeID (typeID) {
        if (version > MAX_VERSION) { throw std::runtime_error ("Version " + std::to_string (version) + " would not fit in " + std::to_string (VERSION_WIDTH) + " bits."); }
        if (typeID > MAX_TYPEID) { throw std::runtime_error ("TypeID " + std::to_string (typeID) + " would not fit in " + std::to_string (TYPEID_WIDTH) + " bits."); }
    }

    /// \brief Virtual destructor, just because I want to derive other classes.
    virtual ~Packet () {}

    /// \brief Adds the version numbers of this and all subpackets.
    /// \return The sum of all version numbers that appears in this packet.
    /// \note Any derived class that has subpackets will need to override this.
    virtual long addVersionNumbers () const { return m_version; }

    /// \brief Evaluates the expression contained within this packet.
    /// \return The value of the expression.
    virtual unsigned long long evaluate () const = 0;

    /// \brief Gets the type of this packet.
    /// \return The tyep of this packet.
    inline unsigned int getTypeID () const { return m_typeID; }

private:
    /// The version of this packet.
    const unsigned int m_version;
    /// The type of this packet.
    const unsigned int m_typeID;
};


// Forward declaration -- see definition below for documentation.
std::unique_ptr<Packet> parse (Bits & bits);


/// \brief A packet that represents a single literal value.
class LiteralPacket : public Packet {
public:
    /// \brief Constructs a literal packet.
    /// \param[in] version The new packet's version.
    /// \param[in] typeID The new packet's type.
    /// \param[inout] bits A bit stream from which more information can be extracted.
    /// \post The literal value has been extracted from the bit stream and all fields set.
    LiteralPacket (unsigned int version, unsigned int typeID, Bits & bits) 
        : Packet (version, typeID), m_value {parseValue (bits)} {
    }
    /// \brief Destructor does not need to do anything.
    virtual ~LiteralPacket () {}

    /// \brief Evaluates this packet.
    /// \return The literal value stored here.
    virtual unsigned long long evaluate () const {
        return m_value;
    }

private:
    /// \brief Parses a literal value from a bit stream.
    /// \param[inout] bits The bit stream.
    /// \return The value.
    /// \post The value will have been extracted from the bit stream.
    static unsigned long long parseValue (Bits & bits) {
        bool finished = false;
        unsigned long long value = 0ULL;
        while (!finished) {
            finished = !bits.front ();
            bits.pop_front ();
            unsigned int nextPart = parseNumber (bits, LITERAL_CHUNK_WIDTH);
            value <<= LITERAL_CHUNK_WIDTH;
            value += nextPart;
        }
        return value;
    }

private:
    /// The literal value stored in this packet.
    const unsigned long long m_value;
};





/// \brief A packet that represents an operator.
/// \note We could derive 7 more classes from this, but that seems like overkill for now.
class OperatorPacket : public Packet {
public:
    /// \brief Constructs an operator packet.
    /// \param[in] version The version of the new packet.
    /// \param[in] typeID The type of the new packet.
    /// \param[inout] bits A bit stream from which the next packet can be extracted.
    /// \post All subpackets have been extracted from the bit stream, all fields set correctly.
    OperatorPacket (unsigned int version, unsigned int typeID, Bits & bits) 
        : Packet (version, typeID), m_subPackets (parseSubpackets (bits)) {
    }

    /// \brief Destructor can just let the member destructor do its thing.
    virtual ~OperatorPacket () {}

    /// \brief Adds the version numbers of this packet and all of its subpackets.
    /// \return The sum of this packet's version numbers and its subpacket's version numbers (with all of their descendants).
    virtual long addVersionNumbers () const {
        long result = Packet::addVersionNumbers ();
        for (std::unique_ptr<const Packet> const& sub : m_subPackets) {
            result += sub->addVersionNumbers ();
        }
        return result;
    }

    /// \brief Evaluates the expression this packet represents.
    /// \return The result of applying this operator to its operands.
    virtual unsigned long long evaluate () const {
        switch (getTypeID ()) {
            case TYPE_SUM: {
                unsigned long long sum {0ULL};
                for (std::unique_ptr<const Packet> const& sub : m_subPackets) {
                    sum += sub->evaluate ();
                }
                return sum;
            }
            case TYPE_PRODUCT: {
                unsigned long long product {1ULL};
                for (std::unique_ptr<const Packet> const& sub : m_subPackets) {
                    product *= sub->evaluate ();
                }
                return product;
            }
            case TYPE_MINIMUM: {
                if (m_subPackets.size () == 0U) { throw std::runtime_error ("A minimum operator packet with no subpackets was found."); }
                unsigned long long min {ULLONG_MAX};
                for (std::unique_ptr<const Packet> const& sub : m_subPackets) {
                    unsigned long long curr = sub->evaluate ();
                    if (curr < min) {
                        min = curr;
                    }
                }
                return min;
            }
            case TYPE_MAXIMUM: {
                if (m_subPackets.size () == 0U) { throw std::runtime_error ("A maximum operator packet with no subpackets was found."); }
                unsigned long long max {0ULL};
                for (std::unique_ptr<const Packet> const& sub : m_subPackets) {
                    unsigned long long curr = sub->evaluate ();
                    if (curr > max) {
                        max = curr;
                    }
                }
                return max;
            }
            case TYPE_GREATER: {
                if (m_subPackets.size () != 2) { throw std::runtime_error ("A greater-than packet did not have 2 subpackets."); }
                return m_subPackets.at (0)->evaluate () > m_subPackets.at (1)->evaluate ();
            }
            case TYPE_LESSER: {
                if (m_subPackets.size () != 2) { throw std::runtime_error ("A less-than packet did not have 2 subpackets."); }
                return m_subPackets.at (0)->evaluate () < m_subPackets.at (1)->evaluate ();
            }
            case TYPE_EQUAL: {
                if (m_subPackets.size () != 2) { throw std::runtime_error ("An equal-to packet did not have 2 subpackets."); }
                return m_subPackets.at (0)->evaluate () == m_subPackets.at (1)->evaluate ();
            }
            default: {
                throw std::runtime_error (std::to_string (getTypeID ()) + " is not an operator type.");
            }
        }
    }
    
private:
    /// \brief Parses a bit stream, extracting subpackets.
    /// \param[inout] bits A bit stream for an operator packet from which version and type ID have already been extracted.
    /// \return A vector of subpackets.
    /// \post Information about the subpackets has been extracted from the bit stream.
    static std::vector<std::unique_ptr<const Packet>> parseSubpackets (Bits & bits) {
        std::vector<std::unique_ptr<const Packet>> subpackets;
        bool lengthTypeID = bits.front ();
        bits.pop_front ();
        if (lengthTypeID == false) {
            unsigned int totalLengthOfSubpackets = parseNumber (bits, LENGTH_OF_SUBPACKETS_WIDTH);
            unsigned int targetBits = bits.size () - totalLengthOfSubpackets;
            while (bits.size () != targetBits) {
                subpackets.push_back (parse (bits));
            }
        }
        else {
            unsigned int numberOfSubpackets = parseNumber (bits, NUMBER_OF_SUBPACKETS_WIDTH);
            for (unsigned int count {0U}; count < numberOfSubpackets; ++count) {
                subpackets.push_back (parse (bits));
            }
        }
        return subpackets;
    }

private:
    /// A vector of subpackets.
    const std::vector<std::unique_ptr<const Packet>> m_subPackets;
};



/// \brief Parses a bit stream into a packet.
/// \param[inout] bits The bit stream.
/// \return A packet.
/// \post The relevant bits have been extracted from the bit stream.
std::unique_ptr<Packet> parse (Bits & bits) {
    unsigned int version = parseNumber (bits, 3);
    unsigned int typeID = parseNumber (bits, 3);
    if (typeID == TYPE_LITERAL) {
        return std::unique_ptr<Packet> (new LiteralPacket (version, typeID, bits));
    }
    else {
        return std::unique_ptr<Packet> (new OperatorPacket (version, typeID, bits));
    }
}

/// \brief Inserts a hexit into a bit stream.
/// \param[out] bits The straem into which the hexit should be inserted.
/// \param[in] num The hexit.
/// \post Four bits representing the hexit have been inserted into the bit stream.
void pushHex (Bits & bits, int num) {
    bits.push_back (num / 8);
    bits.push_back ((num % 8) / 4);
    bits.push_back ((num % 4) / 2);
    bits.push_back (num % 2);
}

/// \brief Reads a hexadecimal value from standard input and converts it to a bit stream.
Bits getInput () {
    std::string line = read<std::string> ();
    Bits result;
    for (char c : line) {
        if ( !( (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ) ) { throw std::runtime_error ("Character " + std::to_string (c) + " does not belong in a hexidecimal value."); }
        if (std::isdigit (c)) {
            pushHex (result, c - '0');
        }
        else if (std::islower (c)) {
            pushHex (result, c - 'a' + 10);
        }
        else {
            pushHex (result, c - 'A' + 10);
        }
    }
    return result;
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Bits bits = getInput ();
    std::unique_ptr<Packet> packet = parse (bits);
    std::cout << packet->addVersionNumbers () << "\n";
    std::cout << packet->evaluate () << "\n";
    return 0;
}