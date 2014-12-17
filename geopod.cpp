#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

enum DataType
{
	DataTypeVoid,
	DataTypeUndefined,
	DataTypeCustom, // data handled based on the identifier
	DataTypeUInt32,
	DataTypeSInt32,
	DataTypeString, // null terminated char string
	DataTypeFloat, // either a float32 or a 16.16 fixed (as determined by the scene options flag)
	DataTypeRGB, // 3 * float
	DataTypeRGBA,
	DataTypeArrayFloat,
	DataTypeArrayUInt32,
	DataTypeArraySInt32,
};

struct BlockMetadata
{
	uint32_t identifier;
	std::string name;
	DataType type;
};

static std::vector<std::vector<BlockMetadata>> blocks =
{
	{
		{ 0, "Unknown", DataTypeUndefined },
	},
	{ // Global Blocks
		{ 1000, "Version", DataTypeString },
		{ 1001, "Scene", DataTypeVoid },
		{ 1002, "Export Options", DataTypeString },
		{ 1003, "History", DataTypeString },
	},
	{ // Scene Blocks
		{ 2000, "Clear Colour", DataTypeRGB },
		{ 2001, "Ambient Colour", DataTypeRGB },
		{ 2002, "Num. Cameras", DataTypeUInt32 },
		{ 2003, "Num. Lights", DataTypeUInt32 },
		{ 2004, "Num. Meshes", DataTypeUInt32 },
		{ 2005, "Num. Nodes", DataTypeUInt32 },
		{ 2006, "Num. Mesh Nodes", DataTypeUInt32 },
		{ 2007, "Num. Textures", DataTypeUInt32 },
		{ 2008, "Num. Materials", DataTypeUInt32 },
		{ 2009, "Num. Frames", DataTypeUInt32 },
		{ 2010, "Camera", DataTypeVoid },
		{ 2011, "Light", DataTypeVoid },
		{ 2012, "Mesh", DataTypeVoid },
		{ 2013, "Node", DataTypeVoid },
		{ 2014, "Texture", DataTypeVoid },
		{ 2015, "Material", DataTypeVoid },
		{ 2016, "Scene Flags", DataTypeUInt32 },
		{ 2017, "FPS", DataTypeUInt32 },
		{ 2018, "Scene User Data", DataTypeUndefined },
		{ 2019, "Units", DataTypeUndefined },
	},
	{ // Material Blocks
		{ 3000, "Material Name", DataTypeString },
		{ 3001, "Diffuse Texture Index", DataTypeSInt32 },
		{ 3002, "Material Opacity", DataTypeFloat },
		{ 3003, "Ambient Colour", DataTypeRGB },
		{ 3004, "Diffuse Colour", DataTypeRGB },
		{ 3005, "Specular Colour", DataTypeRGB },
		{ 3006, "Shininess", DataTypeFloat },
		{ 3007, "Effect File Name", DataTypeString },
		{ 3008, "Effect Name", DataTypeString },
		{ 3009, "Ambient Texture Index", DataTypeSInt32 },
		{ 3010, "Specular Colour Texture Index", DataTypeSInt32 },
		{ 3011, "Specular Level Texture Index", DataTypeSInt32 },
		{ 3012, "Bump Map Texture Index", DataTypeSInt32 },
		{ 3013, "Emissive Texture Index", DataTypeSInt32 },
		{ 3014, "Glossiness Texture Index", DataTypeSInt32 },
		{ 3015, "Opacity Texture Index", DataTypeSInt32 },
		{ 3016, "Reflection Texture Index", DataTypeSInt32 },
		{ 3017, "Refraction Texture Index", DataTypeSInt32 },
		{ 3018, "Blending RGB Source Value", DataTypeCustom },
		{ 3019, "Blending Alpha Source Value", DataTypeCustom },
		{ 3020, "Blending RGB Destination Value", DataTypeCustom },
		{ 3021, "Blending Alpha Destination Value", DataTypeCustom },
		{ 3022, "Blending RGB Operation", DataTypeCustom },
		{ 3023, "Blending Alpha Operation", DataTypeCustom },
		{ 3024, "Blending RGBA Colour", DataTypeRGBA },
		{ 3025, "Blending Factor Array", DataTypeArrayFloat },
		{ 3026, "Material Flags", DataTypeCustom },
		{ 3027, "Material User Data", DataTypeCustom },
	},
	{ // Texture Blocks
		{ 4000, "Texture Name", DataTypeString },
	},
	{ // Node Blocks
		{ 5000, "Node Index", DataTypeSInt32 },
		{ 5001, "Node Name", DataTypeString },
		{ 5002, "Material Index", DataTypeSInt32 },
		{ 5003, "Parent Index", DataTypeSInt32 },
		{ 5007, "Animation Position", DataTypeArrayFloat }, // {x, y, z}[]
		{ 5008, "Animation Rotation", DataTypeArrayFloat }, // quaternion { x, y, z, w }[]
		{ 5009, "Animation Scale", DataTypeArrayFloat }, // {x, y, z, xAxis, yAxis, zAxis, stretch }[]
		{ 5010, "Animation Matrix", DataTypeArrayFloat }, // matrix { float[16] }[]
		{ 5011, "Unknown Matrix", DataTypeArrayFloat }, // matrix { float[16] }[]
		{ 5012, "Animation Flags", DataTypeCustom },
		{ 5013, "Animation Position Index", DataTypeArraySInt32 },
		{ 5014, "Animation Rotation Index", DataTypeArraySInt32 },
		{ 5015, "Animation Scale Index", DataTypeArraySInt32 },
		{ 5016, "Animation Matrix Index", DataTypeArraySInt32 },
		{ 5017, "Node User Data", DataTypeUndefined },
	},
	{ // Mesh Blocks
		{ 6000, "Num. Vertices", DataTypeUInt32 },
		{ 6001, "Num. Faces", DataTypeUInt32 },
		{ 6002, "Num. UVW Channels", DataTypeUInt32 },
		{ 6003, "Vertex Index List", DataTypeVoid },
		{ 6004, "Strip Length", DataTypeArrayUInt32 },
		{ 6005, "Num. Strips", DataTypeUInt32 },
		{ 6006, "Vertex List", DataTypeVoid },
		{ 6007, "Normal List", DataTypeVoid },
		{ 6008, "Tangent List", DataTypeVoid },
		{ 6009, "Binormal List", DataTypeVoid },
		{ 6010, "UVW List", DataTypeVoid },
		{ 6011, "Vertex Colour List", DataTypeVoid },
		{ 6012, "Bone Index List", DataTypeVoid },
		{ 6013, "Bone Weights", DataTypeVoid },
		{ 6014, "Interleaved Data List", DataTypeCustom }, // byte array
		{ 6015, "Bone Batch Index List", DataTypeArrayUInt32 },
		{ 6016, "Num. Bone Indices per Batch", DataTypeArrayUInt32 },
		{ 6017, "Bone Offset per Batch", DataTypeArrayUInt32 },
		{ 6018, "Max. Num. Bones per Batch", DataTypeArrayUInt32 },
		{ 6019, "Num. Bone Batches", DataTypeUInt32 },
		{ 6020, "Unpack Matrix", DataTypeArrayFloat },
	},
	{ // Light Blocks
		{ 7000, "Target Object Index", DataTypeSInt32 },
		{ 7001, "Light Colour", DataTypeArrayFloat },
		{ 7002, "Light Type", DataTypeCustom },
		{ 7003, "Constant Attenuation", DataTypeFloat },
		{ 7004, "Linear Attenuation", DataTypeFloat },
		{ 7005, "Quadratic Attenuation", DataTypeFloat },
		{ 7006, "Falloff Angle", DataTypeFloat },
		{ 7007, "Falloff Exponent", DataTypeFloat },
	},
	{ // Camera Blocks
		{ 8000, "Target Object Index", DataTypeSInt32 },
		{ 8001, "Field of View", DataTypeFloat },
		{ 8002, "Far Plane", DataTypeFloat },
		{ 8003, "Near Plane", DataTypeFloat },
		{ 8004, "FOV Animation", DataTypeArrayFloat },
	},
	{ // POD Data Blocks
		{ 9000, "Data Type", DataTypeCustom },
		{ 9001, "Num. Components", DataTypeUInt32 },
		{ 9002, "Stride", DataTypeUInt32 },
		{ 9003, "Data", DataTypeCustom },
	}
};

static void handle_pod_data(uint32_t identifier, std::vector<unsigned char> data)
{
	BlockMetadata block = blocks[0][0]; // default 'unknown' block
	if (identifier / 1000 < blocks.size())
	{
		for (size_t u = 0; u < sizeof(blocks[identifier / 1000]); ++u)
		{
			if( blocks[identifier / 1000][u].identifier == identifier )
			{
				block = blocks[identifier / 1000][u];
				break;
			}
		}
	}
	std::cout << block.name << ": " << std::setw(0);
	switch (block.type)
	{
		case DataTypeVoid:
			assert(data.empty());
			break;
		case DataTypeUInt32:
			std::cout << *reinterpret_cast<uint32_t*>(data.data());
			break;
		case DataTypeSInt32:
			std::cout << *reinterpret_cast<int32_t*>(data.data());
			break;
		case DataTypeString:
			std::cout << reinterpret_cast<char*>(data.data());
			break;
		case DataTypeFloat:
			std::cout << *reinterpret_cast<float*>(data.data());
			break;
		case DataTypeRGB:
		case DataTypeRGBA:
		case DataTypeArrayFloat:
		{
			float* values = reinterpret_cast<float*>(data.data());
			std::cout << "< ";
			for( size_t u = 0; u < data.size() / sizeof(float); ++u )
			{
				std::cout << values[u] << ", ";
			}
			std::cout << '>';
			break;
		}
		case DataTypeArraySInt32:
		{
			int32_t* values = reinterpret_cast<int32_t*>(data.data());
			std::cout << "< ";
			for( size_t u = 0; u < data.size() / sizeof(int32_t); ++u )
			{
				std::cout << values[u] << ", ";
			}
			std::cout << '>';
			break;
		}
		case DataTypeArrayUInt32:
		{
			uint32_t* values = reinterpret_cast<uint32_t*>(data.data());
			std::cout << "< ";
			for( size_t u = 0; u < data.size() / sizeof(uint32_t); ++u )
			{
				std::cout << values[u] << ", ";
			}
			std::cout << '>';
			break;
		}
		case DataTypeCustom:
		{
			// TODO handle based on identifier
			if( data.size() == sizeof(uint32_t) )
			{
				std::cout << *reinterpret_cast<uint32_t*>(data.data());
				break;
			}
			[[clang::fallthrough]];
		}
		case DataTypeUndefined:
		default:
		{
			if( data.size() > 256 )
			{
				std::cout << "< " << std::dec << data.size() << " bytes >";
				break;
			}
			std::cout << "< ";
			for (unsigned char c : data)
			{
				std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned>(c) << ' ';
			}
			std::cout << '>';
			break;
		}

	}
}

static bool dump_pod_block(std::fstream& stream, std::string indentation)
{
	struct { uint32_t identifier, length; } tag;
	stream.read(reinterpret_cast<char*>(&tag), sizeof(tag));

	if (!stream || (tag.identifier & 0x80000000))
	{
		// this tag is the end of a block - return
		assert(tag.length == 0);
		return false;
	}
	else
	{
		std::cout << indentation << std::dec << std::setw(4) << tag.identifier << ": ";

		// this tag is the start of a block - print its data and recurse down
		std::vector<unsigned char> data(tag.length);
		stream.read(reinterpret_cast<char*>(data.data()), data.size());

		handle_pod_data(tag.identifier, data);
		std::cout << std::endl;

		while (dump_pod_block(stream, indentation + '\t')) {}
		return true;
	}
}

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::fstream stream(argv[1], std::ios::in | std::ios::binary);

	while (stream.good())
	{
		dump_pod_block(stream, "");
	}

	if (!stream.eof())
	{
		std::cerr << "Error: " << strerror(errno);
		return 1;
	}
}
