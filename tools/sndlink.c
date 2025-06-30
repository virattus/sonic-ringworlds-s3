#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


const char arghelpDash[] = "-h";
const char arghelpFull[] = "--help";
const char argVerboseDash[] = "-v";
const char argVerboseFull[] = "--verbose";

const char argADPCM[] = "-a";
const char argPCM[] = "-p";
const char argOutput[] = "-o";

#define SOUND_FILE_COUNT_MAX	(256)
#define SOUND_FILE_SIZE_MAX		(0xFFFF)
#define FILE_NAME_SIZE_MAX		(256)

#define BYTE_COUNT_MAX			(0x80000)
#define DRIVER_BYTE_SIZE		(0x2000)


#define READ_STATE_ADPCM		(0)
#define READ_STATE_PCM			(1)
#define READ_STATE_OUTPUT		(2)


typedef struct
{
	uint16_t FileOffset;
	uint16_t FileLength;
	uint16_t FileLoopStart; //Remember to write these as absolute addresses
	uint16_t FileLoopEnd; //This too
	char* FileName;
	char* FileData;
	
} SoundFileEntry;


int readingState = READ_STATE_ADPCM;

int ADPCMFileIndices[SOUND_FILE_COUNT_MAX];
int ADPFileCount = 0;
int PCMFileIndices[SOUND_FILE_COUNT_MAX];
int PCMFileCount = 0;

int TotalFileSize = 0;

char OutputFilename[FILE_NAME_SIZE_MAX];

SoundFileEntry Entries[SOUND_FILE_COUNT_MAX * 2];
int EntryCount = 0;


bool VerboseMode = false;
bool FileCreationError = false;


bool IsADPMaxed(void)
{
	return ADPFileCount >= SOUND_FILE_COUNT_MAX;
}

bool IsPCMMaxed(void)
{
	return PCMFileCount >= SOUND_FILE_COUNT_MAX;
}


uint16_t SwapEndianness_16(uint16_t val)
{
	return ((val & 0x00FF) << 8) + ((val & 0xFF00) >> 8);
}


bool AddFile(const char* location)
{	
	FILE* soundfile = fopen(location, "rb");
	if(soundfile == NULL)
	{
		printf("Error: unable to read input file %s\n", location);
		return false;
	}
	
	if(VerboseMode)
	{
		printf("Reading in file %s\n", location);
	}
	
	int filesize = 0;
	
	fseek(soundfile, 0L, SEEK_END);
	filesize = ftell(soundfile);
	//rewind(soundfile);
	fseek(soundfile, 0L, SEEK_SET);
	
	if(VerboseMode)
	{
		printf("File %s was found to be of length %d\n", location, filesize);
	}
	
	if(filesize == 0)
	{
		printf("Error: file %s is a zero length file\n", location);
		fclose(soundfile);
		return false;
	}
	
	if(filesize > SOUND_FILE_SIZE_MAX)
	{
		printf("error: file %s is too large, max file size of %d\n", location, SOUND_FILE_SIZE_MAX);
		fclose(soundfile);
		return false;
	}
	
	if(filesize + TotalFileSize > BYTE_COUNT_MAX)
	{
		printf("Error: total files recorded for packing exceed max SCSP RAM. Offending files start at %s\n", location); 
		fclose(soundfile);
		return false;
	}

	
	SoundFileEntry* entry = &Entries[EntryCount];
	
	entry->FileLength = filesize;
	
	entry->FileData = (char*)malloc(entry->FileLength);
	const size_t readSize = fread(entry->FileData, 1, entry->FileLength, soundfile);
	
	if(readSize != filesize)
	{
		printf("Error: amount read doesn't match size of file\n");
		fclose(soundfile);
		free(entry->FileData);
		return false;
	}
	
	fclose(soundfile);
	
	
	//Copy over filename for later
	size_t filenameSize = strlen(location);
	entry->FileName = (char*)malloc(filenameSize);
	strcpy(entry->FileName, location);
	
	entry->FileOffset = TotalFileSize;
	TotalFileSize += entry->FileLength;

	
	return true;
}


bool AddADPCM(const char* location)
{
	if(IsADPMaxed())
	{
		printf("error: too many ADPCM files, only a max of %s allowed\n",  SOUND_FILE_COUNT_MAX);
		return false;
	}
	
	if(!AddFile(location))
	{
		printf("Failed to add ADPCM %s\n", location);
		return false;
	}
	
	int index = EntryCount;
	EntryCount += 1;
	
	if(VerboseMode)
	{
		printf("Adding %s to ADPCM list at index %d\n", location, index);
	}

	
	ADPCMFileIndices[ADPFileCount] = index;
	ADPFileCount += 1;
	
	return true;
}


bool AddPCM(const char* location)
{
	if(IsPCMMaxed())
	{
		printf("error: too many PCM files, only a max of $s allowed\n", SOUND_FILE_COUNT_MAX);
		return false;
	}
		
	if(!AddFile(location))
	{
		printf("Failed to add PCM %s\n", location);
		return false;
	}
	
	int index = EntryCount;
	EntryCount += 1;
	
	if(VerboseMode)
	{
		printf("Adding %s to PCM list at index %d\n", location, index);
	}
	
	PCMFileIndices[PCMFileCount] = index;
	PCMFileCount += 1;
	
	return true;
}




bool WriteOutputFile(void)
{
	if(!OutputFilename[0])
	{
		printf("Error: No output file\n");
		return false;
	}
	
	
	FILE* output = fopen(OutputFilename, "wb");
	if(output == NULL)
	{
		printf("Error: unable to open output file %s\n", OutputFilename);
		return false;
	}
	
	uint16_t RAMOffset = DRIVER_BYTE_SIZE + ((sizeof(uint16_t) * 2) * (ADPFileCount + PCMFileCount));
	
	// CREATE FILE OFFSET TABLE
	for(int i = 0; i < ADPFileCount; i++)
	{
		SoundFileEntry* entry = &Entries[ADPCMFileIndices[i]];
		
		uint16_t offset = SwapEndianness_16(entry->FileOffset + RAMOffset);
		uint16_t length = SwapEndianness_16(entry->FileLength);
		
		fwrite(&offset, sizeof(uint16_t), 1, output);
		fwrite(&length, sizeof(uint16_t), 1, output);
	}
	
	
	for(int i = 0; i < PCMFileCount; i++)
	{
		SoundFileEntry* entry = &Entries[PCMFileIndices[i]];
		
		uint16_t offset = SwapEndianness_16(entry->FileOffset + RAMOffset);
		uint16_t length = SwapEndianness_16(entry->FileLength);
		
		fwrite(&offset, sizeof(uint16_t), 1, output);
		fwrite(&length, sizeof(uint16_t), 1, output);
	}
	// END CREATE FILE OFFSET TABLE
	
	uint16_t index = 0;
	
	// OUTPUT FILE DATA
	for(int i = 0; i < ADPFileCount; i++)
	{
		SoundFileEntry* entry = &Entries[ADPCMFileIndices[i]];
	
		size_t retval = fwrite(entry->FileData, 1, entry->FileLength, output);
		
		if(retval != entry->FileLength)
		{
			printf("ERROR: fwrite fucked up: %d %d\n", entry->FileLength, retval);
			fclose(output);
			return false;
		}
		
		printf("INDEX: %d FILENAME: %s SIZE: %d ADPCM\n", index, entry->FileName, entry->FileLength);
		//printf("%s\n", entry->FileData);

		index++;
	}
	
	for(int i = 0; i < PCMFileCount; i++)
	{
		SoundFileEntry* entry = &Entries[PCMFileIndices[i]];
	
		size_t retval = fwrite(entry->FileData, 1, entry->FileLength, output);
		
		if(retval != entry->FileLength)
		{
			printf("ERROR: fwrite fucked up: %d %d\n", entry->FileLength, retval);
			fclose(output);
			return false;
		}
		
		printf("INDEX: %d FILENAME: %s SIZE: %d PCM\n", index, entry->FileName, entry->FileLength);
		//printf("%s\n", entry->FileData);

		index++;
	}
	
	
	
	fclose(output);
	
	return true;
}


int main(int argc, char** argv)
{
	if(argc < 2 || 
		!strcmp(argv[1], arghelpDash) || 
		!strcmp(argv[1], arghelpFull))
	{
		printf("Sonic Ringworlds Sound Linker\n"
			"Arguments: -v (verbose) -a { ADPCM Files} -p {PCM Files} -o (output filename)\n");
		return 0;
	}
	
	
	OutputFilename[0] = 0; //Closest thing I can think of to a NULL right now
	
	
	for(int i = 1; i < argc; i++)
	{
		if(readingState == READ_STATE_OUTPUT)
		{
			if(strlen(argv[i]) > FILE_NAME_SIZE_MAX)
			{
				printf("Error: output filename and directory path greater than FILENAME_SIZE_MAX characters\n");
				FileCreationError = true;
				break;
			}

			if(VerboseMode)
			{
				printf("output filename: %s\n", argv[i]);
			}
			
			strcpy(OutputFilename, argv[i]);
			break;
		}
		
		if(!strcmp(argv[i], argVerboseDash) ||
			!strcmp(argv[i], argVerboseFull))
		{
			VerboseMode = true;
			continue;
		}
		
		if(!strcmp(argv[i], argOutput))
		{
			readingState = READ_STATE_OUTPUT;
			continue;
		}
		
		if(!strcmp(argv[i], argADPCM))
		{
			readingState = READ_STATE_ADPCM;
			continue;
		}
		
		if(!strcmp(argv[i], argPCM))
		{
			readingState = READ_STATE_PCM;
			continue;
		}
		
		if(readingState == READ_STATE_ADPCM)
		{
			if(!AddADPCM(argv[i]))
			{
				FileCreationError = true;
				break;
			}
		}
		
		if(readingState == READ_STATE_PCM)
		{
			if(!AddPCM(argv[i]))
			{
				FileCreationError = true;
				break;
			}
		}
	}
	
	
	if(!FileCreationError)
	{
		FileCreationError = !WriteOutputFile();
	}
	
	if(FileCreationError)
	{
		printf("----SNDLINK ERROR: FAILED TO CREATE SOUND FILE----\n");
	}
	
	
	return 0;
}
