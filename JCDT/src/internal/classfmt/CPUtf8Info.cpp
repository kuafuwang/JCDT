
#include <JCDT_Lib/internal/classfmt/CPUtf8Info.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	CPUtf8Info::CPUtf8Info(ClassFile& buffer)
		: CPInfo(CONSTANT_Utf8)
		, len(buffer.GetU2())
		, valid(true)
	{
		u2 size = (u2)buffer.GetN(bytes, len);
		if (size != len)
			valid = false;
		Init(size);
		if (!valid)
			buffer.MarkInvalid("bad CPUtf8Info");
	}

	void CPUtf8Info::Init(u2 size)
	{
#ifdef JIKES_DEBUG
		const char* tmp;
#endif // JIKES_DEBUG
		for (u2 i = 0; i < size; i++)
		{
			switch (bytes[i])
			{
			case 0x00:
			case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5:
			case 0xf6: case 0xf7: case 0xf8: case 0xf9: case 0xfa: case 0xfb:
			case 0xfc: case 0xfd: case 0xfe: case 0xff: // invalid
				valid = false;
#ifdef JIKES_DEBUG
				contents.push_back('\\');
				contents.push_back('x');
				tmp = IntToString(bytes[i], 2).String();
				contents.push_back(tmp[0]);
				contents.push_back(tmp[1]);
#endif // JIKES_DEBUG
				break;
#ifdef JIKES_DEBUG
			case 0x09:
				contents.push_back('\\');
				contents.push_back('t');
				break;
			case 0x0a:
				contents.push_back('\\');
				contents.push_back('n');
				break;
			case 0x0d:
				contents.push_back('\\');
				contents.push_back('r');
				break;
			case 0x22:
				contents.push_back('\\');
				contents.push_back('"');
				break;
			case 0x5c:
				contents.push_back('\\');
				contents.push_back('\\');
				break;
			case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06:
			case 0x07: case 0x08: case 0x0b: case 0x0c: case 0x0e: case 0x0f:
			case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15:
			case 0x16: case 0x17: case 0x18: case 0x19: case 0x1a: case 0x1b:
			case 0x1c: case 0x1d: case 0x1e: case 0x1f: case 0x7f:
				// non-printing ASCII
				contents.push_back('\\');
				contents.push_back('u');
				tmp = IntToString(bytes[i], 4).String();
				contents.push_back(tmp[0]);
				contents.push_back(tmp[1]);
				contents.push_back(tmp[2]);
				contents.push_back(tmp[3]);
				break;
#endif // JIKES_DEBUG
			default:
				if (bytes[i] <= 0x7f) // 1-byte (printing ASCII, if JIKES_DEBUG)
				{
#ifdef JIKES_DEBUG
					contents.push_back(bytes[i]);
#endif // JIKES_DEBUG
				}
				else if (bytes[i] <= 0xdf) // 2-byte source
				{
					if (i + 1 == size || (bytes[i + 1] & 0xc0) != 0x80)
					{
						valid = false;
#ifdef JIKES_DEBUG
						contents.push_back('\\');
						contents.push_back('x');
						tmp = IntToString(bytes[i], 2).String();
						contents.push_back(tmp[0]);
						contents.push_back(tmp[1]);
#endif // JIKES_DEBUG
						break;
					}
					++i;
#ifdef JIKES_DEBUG
					u2 value = (bytes[i - 1] & 0x1f) << 6;
					value |= bytes[i] & 0x3f;
					contents.push_back('\\');
					contents.push_back('u');
					tmp = IntToString(value, 4).String();
					contents.push_back(tmp[0]);
					contents.push_back(tmp[1]);
					contents.push_back(tmp[2]);
					contents.push_back(tmp[3]);
#endif // JIKES_DEBUG
				}
				else // 3-byte source
				{
					assert((bytes[i] & 0xf0) == 0xe0);
					if (i + 2 >= size ||
						(bytes[i + 1] & 0xc0) != 0x80 ||
						(bytes[i + 2] & 0xc0) != 0x80)
					{
						valid = false;
#ifdef JIKES_DEBUG
						contents.push_back('\\');
						contents.push_back('x');
						tmp = IntToString(bytes[i], 2).String();
						contents.push_back(tmp[0]);
						contents.push_back(tmp[1]);
#endif // JIKES_DEBUG
						break;
					}
					i += 2;
#ifdef JIKES_DEBUG
					u2 value = (bytes[i - 2] & 0x0f) << 12;
					value |= (bytes[i - 1] & 0x3f) << 6;
					value |= bytes[i] & 0x3f;
					contents.push_back('\\');
					contents.push_back('u');
					tmp = IntToString(value, 4).String();
					contents.push_back(tmp[0]);
					contents.push_back(tmp[1]);
					contents.push_back(tmp[2]);
					contents.push_back(tmp[3]);
#endif // JIKES_DEBUG
				}
			}
		}
#ifdef JIKES_DEBUG
		if (!valid)
		{
			contents.push_back('\\');
			contents.push_back('i');
			contents.push_back('n');
			contents.push_back('v');
			contents.push_back('a');
			contents.push_back('l');
			contents.push_back('i');
			contents.push_back('d');
			contents.push_back('\\');
		}
		contents.push_back(0);
#endif // JIKES_DEBUG
	}

	string CPUtf8Info::GetContents()
	{
		return contents.c_str();
	}

	void CPUtf8Info::Print(Ostream& os, const ConstantPool&) const
	{
		os << "Utf8: length: " << (unsigned)len
			<< ", bytes[]: " << contents.c_str() << endl;
	}

	void CPUtf8Info::Describe(Ostream& os, const ConstantPool&) const
	{
		os << contents.c_str();
	}
} // Close namespace Jikes block



