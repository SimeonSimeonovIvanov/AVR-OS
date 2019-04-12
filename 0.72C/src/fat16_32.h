/*! \file fat.h \brief FAT16/32 file system driver. */
//*****************************************************************************
//
// File Name	: 'fat.h'
// Title		: FAT16/32 file system driver
// Author		: Pascal Stang
// Date			: 11/07/2000
// Revised		: 12/12/2000
// Version		: 0.3
// Target MCU	: ATmega103 (should work for Atmel AVR Series)
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup general
/// \defgroup fat FAT16/32 File System Interface (fat.c)
/// \code #include "fat.h" \endcode
/// \par Overview
///		This FAT16/32 interface allows you to detect and mount FAT16/32
///		partitions, browse directories and files, and read file data.
///		The interface is designed to operate with the avrlib IDE/ATA driver.
///		Reading FAT efficiently requires at least 512+ bytes of RAM so this
///		interface may not be suitable for processors with less than 1K of RAM.
///		This interface will properly follow a file's cluster chain so files
///		need not be defragmented.
///
/// \note This code is based in part on work done by Jesper Hansen for his
///		excellent YAMPP MP3 player project.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef FAT_H
#define FAT_H

#include "global.h"


// Some useful cluster numbers
#define MSDOSFSROOT     0               // cluster 0 means the root dir
#define CLUST_FREE      0               // cluster 0 also means a free cluster
#define MSDOSFSFREE     CLUST_FREE
#define CLUST_FIRST     2               // first legal cluster number
#define CLUST_RSRVD     0xfffffff6      // reserved cluster range
#define CLUST_BAD       0xfffffff7      // a cluster with a defect
#define CLUST_EOFS      0xfffffff8      // start of eof cluster range
#define CLUST_EOFE      0xffffffff      // end of eof cluster range

#define FAT12_MASK      0x00000fff      // mask for 12 bit cluster numbers
#define FAT16_MASK      0x0000ffff      // mask for 16 bit cluster numbers
#define FAT32_MASK      0x0fffffff      // mask for FAT32 cluster numbers


// Partition Type used in the partition record
#define PART_TYPE_UNKNOWN		0x00
#define PART_TYPE_FAT12			0x01
#define PART_TYPE_XENIX			0x02
#define PART_TYPE_DOSFAT16		0x04
#define PART_TYPE_EXTDOS		0x05
#define PART_TYPE_FAT16			0x06
#define PART_TYPE_NTFS			0x07
#define PART_TYPE_FAT32			0x0B
#define PART_TYPE_FAT32LBA		0x0C
#define PART_TYPE_FAT16LBA		0x0E
#define PART_TYPE_EXTDOSLBA		0x0F
#define PART_TYPE_ONTRACK		0x33
#define PART_TYPE_NOVELL		0x40
#define PART_TYPE_PCIX			0x4B
#define PART_TYPE_PHOENIXSAVE	0xA0
#define PART_TYPE_CPM			0xDB
#define PART_TYPE_DBFS			0xE0
#define PART_TYPE_BBT			0xFF

struct partrecord // length 16 bytes
{			
	BYTE	prIsActive;					// 0x80 indicates active partition
	BYTE	prStartHead;				// starting head for partition
	WORD	prStartCylSect;				// starting cylinder and sector
	BYTE	prPartType;					// partition type (see above)
	BYTE	prEndHead;					// ending head for this partition
	WORD	prEndCylSect;				// ending cylinder and sector
	DWORD	prStartLBA;					// first LBA sector for this partition
	DWORD	prSize;						// size of this partition (bytes or sectors ?)
};

        
struct partsector
{
	CHAR	psPartCode[512-64-2];		// pad so struct is 512b
	BYTE	psPart[64];					// four partition records (64 bytes)
	BYTE	psBootSectSig0;				// two signature bytes (2 bytes)
	BYTE	psBootSectSig1;
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
};



// Format of a boot sector.  This is the first sector on a DOS floppy disk
// or the first sector of a partition on a hard disk.  But, it is not the
// first sector of a partitioned hard disk.
struct bootsector33 {
	BYTE 	bsJump[3];					// jump inst E9xxxx or EBxx90
	CHAR	bsOemName[8];				// OEM name and version
	CHAR	bsBPB[19];					// BIOS parameter block
	CHAR	bsDriveNumber;				// drive number (0x80)
	CHAR	bsBootCode[479];			// pad so struct is 512b
	BYTE	bsBootSectSig0;				// boot sector signature byte 0x55
	BYTE	bsBootSectSig1;				// boot sector signature byte 0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
};

struct extboot {
	CHAR	exDriveNumber;				// drive number (0x80)
	CHAR	exReserved1;				// reserved
	CHAR	exBootSignature;			// ext. boot signature (0x29)
#define EXBOOTSIG       0x29
	CHAR	exVolumeID[4];				// volume ID number
	CHAR	exVolumeLabel[11];			// volume label
	CHAR	exFileSysType[8];			// fs type (FAT12 or FAT16)
};

struct bootsector50 {
	BYTE	bsJump[3];					// jump inst E9xxxx or EBxx90
	CHAR	bsOemName[8];				// OEM name and version
	CHAR	bsBPB[25];					// BIOS parameter block
	CHAR	bsExt[26];					// Bootsector Extension
	CHAR	bsBootCode[448];			// pad so structure is 512b
	BYTE	bsBootSectSig0;				// boot sector signature byte 0x55 
	BYTE	bsBootSectSig1;				// boot sector signature byte 0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
};

struct bootsector710 {
	BYTE	bsJump[3];					// jump inst E9xxxx or EBxx90
	CHAR	bsOEMName[8];				// OEM name and version
	CHAR	bsBPB[53];					// BIOS parameter block
	CHAR	bsExt[26];					// Bootsector Extension
	CHAR	bsBootCode[418];			// pad so structure is 512b
	BYTE	bsBootSectSig2;				// 2 & 3 are only defined for FAT32?
	BYTE	bsBootSectSig3;
	BYTE	bsBootSectSig0;				// boot sector signature byte 0x55
	BYTE	bsBootSectSig1;				// boot sector signature byte 0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
#define BOOTSIG2        0
#define BOOTSIG3        0
};


/***************************************************************/
/***************************************************************/

// BIOS Parameter Block (BPB) for DOS 3.3
struct bpb33 {
        WORD	bpbBytesPerSec; // bytes per sector
        BYTE    bpbSecPerClust;	// sectors per cluster
        WORD	bpbResSectors;	// number of reserved sectors
        BYTE	bpbFATs;		// number of FATs
        WORD	bpbRootDirEnts;	// number of root directory entries
        WORD	bpbSectors;		// total number of sectors
        BYTE	bpbMedia;		// media descriptor
        WORD	bpbFATsecs;     // number of sectors per FAT
        WORD	bpbSecPerTrack; // sectors per track
        WORD	bpbHeads;       // number of heads
        WORD	bpbHiddenSecs;  // number of hidden sectors
};

// BPB for DOS 5.0
// The difference is bpbHiddenSecs is a short for DOS 3.3,
// and bpbHugeSectors is not present in the DOS 3.3 bpb.
struct bpb50 {
        WORD	bpbBytesPerSec; // bytes per sector
        BYTE	bpbSecPerClust; // sectors per cluster
        WORD	bpbResSectors;  // number of reserved sectors
        BYTE	bpbFATs;        // number of FATs
        WORD	bpbRootDirEnts; // number of root directory entries
        WORD	bpbSectors;     // total number of sectors
        BYTE	bpbMedia;       // media descriptor
        WORD	bpbFATsecs;     // number of sectors per FAT
        WORD	bpbSecPerTrack; // sectors per track
        WORD	bpbHeads;       // number of heads
        DWORD	bpbHiddenSecs;  // # of hidden sectors
// 3.3 compat ends here
        DWORD	bpbHugeSectors; // # of sectors if bpbSectors == 0
};

// BPB for DOS 7.10 (FAT32)
// This one has a few extensions to bpb50.
struct bpb710 {
		WORD	bpbBytesPerSec;	// bytes per sector
		BYTE	bpbSecPerClust;	// sectors per cluster
		WORD	bpbResSectors;	// number of reserved sectors
		BYTE	bpbFATs;		// number of FATs
		WORD	bpbRootDirEnts;	// number of root directory entries
		WORD	bpbSectors;		// total number of sectors
		BYTE	bpbMedia;		// media descriptor
		WORD	bpbFATsecs;		// number of sectors per FAT
		WORD	bpbSecPerTrack;	// sectors per track
		WORD	bpbHeads;		// number of heads
		DWORD	bpbHiddenSecs;	// # of hidden sectors
// 3.3 compat ends here
		DWORD	bpbHugeSectors;	// # of sectors if bpbSectors == 0
// 5.0 compat ends here
		DWORD     bpbBigFATsecs;// like bpbFATsecs for FAT32
		WORD      bpbExtFlags;	// extended flags:
#define FATNUM    0xf			// mask for numbering active FAT
#define FATMIRROR 0x80			// FAT is mirrored (like it always was)
		WORD      bpbFSVers;	// filesystem version
#define FSVERS    0				// currently only 0 is understood
		DWORD     bpbRootClust;	// start cluster for root directory
		WORD      bpbFSInfo;	// filesystem info structure sector
		WORD      bpbBackup;	// backup boot sector
		// There is a 12 byte filler here, but we ignore it
};




// ***************************************************************
// * byte versions of the above structs                          *
// ***************************************************************


// BIOS Parameter Block (BPB) for DOS 3.3
struct byte_bpb33 {
        CHAR bpbBytesPerSec[2];		// bytes per sector
        CHAR bpbSecPerClust;        // sectors per cluster
        CHAR bpbResSectors[2];      // number of reserved sectors
        CHAR bpbFATs;               // number of FATs
        CHAR bpbRootDirEnts[2];     // number of root directory entries
        CHAR bpbSectors[2];         // total number of sectors
        CHAR bpbMedia;              // media descriptor
        CHAR bpbFATsecs[2];         // number of sectors per FAT
        CHAR bpbSecPerTrack[2];     // sectors per track
        CHAR bpbHeads[2];           // number of heads
        CHAR bpbHiddenSecs[2];      // number of hidden sectors
};

// BPB for DOS 5.0
// The difference is bpbHiddenSecs is a short for DOS 3.3,
// and bpbHugeSectors is not in the 3.3 bpb.
struct byte_bpb50 {
        CHAR bpbBytesPerSec[2];     // bytes per sector
        CHAR bpbSecPerClust;        // sectors per cluster
        CHAR bpbResSectors[2];      // number of reserved sectors
        CHAR bpbFATs;               // number of FATs
        CHAR bpbRootDirEnts[2];     // number of root directory entries
        CHAR bpbSectors[2];         // total number of sectors
        CHAR bpbMedia;              // media descriptor
        CHAR bpbFATsecs[2];         // number of sectors per FAT
        CHAR bpbSecPerTrack[2];     // sectors per track
        CHAR bpbHeads[2];           // number of heads
        CHAR bpbHiddenSecs[4];      // number of hidden sectors
        CHAR bpbHugeSectors[4];		// # of sectors if bpbSectors == 0
};

// BPB for DOS 7.10 (FAT32).
// This one has a few extensions to bpb50.
struct byte_bpb710 {
        BYTE bpbBytesPerSec[2];     // bytes per sector
        BYTE bpbSecPerClust;        // sectors per cluster
        BYTE bpbResSectors[2];      // number of reserved sectors
        BYTE bpbFATs;               // number of FATs
        BYTE bpbRootDirEnts[2];     // number of root directory entries
        BYTE bpbSectors[2];         // total number of sectors
        BYTE bpbMedia;              // media descriptor
        BYTE bpbFATsecs[2];         // number of sectors per FAT
        BYTE bpbSecPerTrack[2];     // sectors per track
        BYTE bpbHeads[2];           // number of heads
        BYTE bpbHiddenSecs[4];      // # of hidden sectors
        BYTE bpbHugeSectors[4];     // # of sectors if bpbSectors == 0
        BYTE bpbBigFATsecs[4];      // like bpbFATsecs for FAT32
        BYTE bpbExtFlags[2];        // extended flags:
        BYTE bpbFSVers[2];          // filesystem version
        BYTE bpbRootClust[4];       // start cluster for root directory
        BYTE bpbFSInfo[2];          // filesystem info structure sector
        BYTE bpbBackup[2];          // backup boot sector
        // There is a 12 byte filler here, but we ignore it
};

// FAT32 FSInfo block.
struct fsinfo {
        BYTE fsisig1[4];
        BYTE fsifill1[480];
        BYTE fsisig2[4];
        BYTE fsinfree[4];
        BYTE fsinxtfree[4];
        BYTE fsifill2[12];
        BYTE fsisig3[4];
        BYTE fsifill3[508];
        BYTE fsisig4[4];
};


/***************************************************************/
/***************************************************************/


// Structure of a dos directory entry.
struct direntry {
		BYTE		deName[8];      // filename, blank filled
#define SLOT_EMPTY      0x00            // slot has never been used
#define SLOT_E5         0x05            // the real value is 0xE5
#define SLOT_DELETED    0xE5            // file in this slot deleted
		BYTE		deExtension[3]; // extension, blank filled
		BYTE		deAttributes;   // file attributes
#define ATTR_NORMAL     0x00            // normal file
#define ATTR_READONLY   0x01            // file is readonly
#define ATTR_HIDDEN     0x02            // file is hidden
#define ATTR_SYSTEM     0x04            // file is a system file
#define ATTR_VOLUME     0x08            // entry is a volume label
#define ATTR_LONG_FILENAME	0x0F		// this is a long filename entry			    
#define ATTR_DIRECTORY  0x10            // entry is a directory name
#define ATTR_ARCHIVE    0x20            // file is new or modified
		BYTE        deLowerCase;    // NT VFAT lower case flags  (set to zero)
#define LCASE_BASE      0x08            // filename base in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
		BYTE        deCHundredth;   // hundredth of seconds in CTime
		BYTE        deCTime[2];     // create time
		BYTE        deCDate[2];     // create date
		BYTE        deADate[2];     // access date
		WORD        deHighClust; 	// high bytes of cluster number
		BYTE        deMTime[2];     // last update time
		BYTE        deMDate[2];     // last update date
		WORD        deStartCluster; // starting cluster of file
		DWORD       deFileSize;  	// size of file in bytes
};

// number of directory entries in one sector
#define DIRENTRIES_PER_SECTOR	0x10

// Structure of a Win95 long name directory entry
struct winentry {
		BYTE		weCnt;			// 
#define WIN_LAST        0x40
#define WIN_CNT         0x3f
		BYTE		wePart1[10];
		BYTE		weAttributes;
#define ATTR_WIN95      0x0f
		BYTE		weReserved1;
		BYTE		weChksum;
		BYTE		wePart2[12];
		WORD       	weReserved2;
		BYTE		wePart3[4];
};

#define WIN_ENTRY_CHARS	13      // Number of chars per winentry

// Maximum filename length in Win95
// Note: Must be < sizeof(dirent.d_name)
#define WIN_MAXLEN      255

// This is the format of the contents of the deTime field in the direntry
// structure.
// We don't use bitfields because we don't know how compilers for
// arbitrary machines will lay them out.
#define DT_2SECONDS_MASK        0x1F    // seconds divided by 2
#define DT_2SECONDS_SHIFT       0
#define DT_MINUTES_MASK         0x7E0   // minutes
#define DT_MINUTES_SHIFT        5
#define DT_HOURS_MASK           0xF800  // hours
#define DT_HOURS_SHIFT          11

// This is the format of the contents of the deDate field in the direntry
// structure.
#define DD_DAY_MASK				0x1F	// day of month
#define DD_DAY_SHIFT			0
#define DD_MONTH_MASK			0x1E0	// month
#define DD_MONTH_SHIFT			5
#define DD_YEAR_MASK			0xFE00	// year - 1980
#define DD_YEAR_SHIFT			9

// Stuctures
struct FileInfoStruct
{
	unsigned long StartCluster;			//< file starting cluster for last file accessed
	unsigned long Size;					//< file size for last file accessed
	unsigned char Attr;					//< file attr for last file accessed
	unsigned short CreateTime;			//< file creation time for last file accessed
	unsigned short CreateDate;			//< file creation date for last file accessed
};

// Prototypes
unsigned char fatInit( unsigned char device);
unsigned int fatClusterSize(void);
unsigned char fatGetDirEntry(unsigned short entry);
unsigned char fatChangeDirectory(unsigned short entry);
void fatPrintDirEntry(void);
void fatDumpDirSlot(unsigned short entry);
struct FileInfoStruct* fatGetFileInfo(void);
unsigned long fatGetFilesize(void);
char* fatGetFilename(void);
char* fatGetDirname(void);
void fatLoadCluster(unsigned long cluster, unsigned char *buffer);
unsigned long fatNextCluster(unsigned long cluster);




// globals
// buffers
unsigned char *SectorBuffer  =		(unsigned char *) FAT_SECTOR_BUFFER_ADDR;
unsigned char *FileNameBuffer =		(unsigned char *) FAT_FILENAME_BUFFER_ADDR;
unsigned char *PathNameBuffer =		(unsigned char *) FAT_PATHNAME_BUFFER_ADDR;

// filesystem constants/metrics
struct partrecord PartInfo;
unsigned char Fat32Enabled;
unsigned long FirstDataSector;
unsigned short BytesPerSector;
unsigned short SectorsPerCluster;
unsigned long FirstFATSector;
unsigned long RootDirStartCluster;

// operating variables
unsigned long CurrentDirStartCluster;	//< current directory starting cluster
struct FileInfoStruct FileInfo;			//< file information for last file accessed
unsigned long FatInCache = 0;


/*************************************************************************/
/*************************************************************************/

#undef DEBUG_FAT

unsigned long fatClustToSect(unsigned long clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;
}

unsigned int fatClusterSize(void)
{
	// return the number of sectors in a disk cluster
	return SectorsPerCluster;
}

unsigned char fatInit( unsigned char device)
{
	//struct partrecord *pr;
	struct bpb710 *bpb;

	// read partition table
	// TODO.... error checking
	ataReadSectors(DRIVE0, 0, 1, SectorBuffer);
	// map first partition record	
	// save partition information to global PartInfo
	PartInfo = *((struct partrecord *) ((struct partsector *) SectorBuffer)->psPart);
//	PartInfo = *pr;
	
	// Read the Partition BootSector
	// **first sector of partition in PartInfo.prStartLBA
	ataReadSectors( DRIVE0, PartInfo.prStartLBA, 1, SectorBuffer );
	bpb = (struct bpb710 *) ((struct bootsector710 *) SectorBuffer)->bsBPB;

	// setup global disk constants
	FirstDataSector	= PartInfo.prStartLBA;
	if(bpb->bpbFATsecs)
	{
		// bpbFATsecs is non-zero and is therefore valid
		FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * bpb->bpbFATsecs;
	}
	else
	{
		// bpbFATsecs is zero, real value is in bpbBigFATsecs
		FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * bpb->bpbBigFATsecs;
	}
	SectorsPerCluster	= bpb->bpbSecPerClust;
	BytesPerSector		= bpb->bpbBytesPerSec;
	FirstFATSector		= bpb->bpbResSectors + PartInfo.prStartLBA;

	switch (PartInfo.prPartType)
	{
		case PART_TYPE_DOSFAT16:
		case PART_TYPE_FAT16:
		case PART_TYPE_FAT16LBA:
			// first directory cluster is 2 by default (clusters range 2->big)
			RootDirStartCluster = CLUST_FIRST;
			// push data sector pointer to end of root directory area
			//FirstDataSector += (bpb->bpbRootDirEnts)/DIRENTRIES_PER_SECTOR;
			Fat32Enabled = FALSE;
			break;
		case PART_TYPE_FAT32LBA:
		case PART_TYPE_FAT32:
			// bpbRootClust field exists in FAT32 bpb710, but not in lesser bpb's
			RootDirStartCluster = bpb->bpbRootClust;
			// push data sector pointer to end of root directory area
			// need this? FirstDataSector += (bpb->bpbRootDirEnts)/DIRENTRIES_PER_SECTOR;
			Fat32Enabled = TRUE;
			break;
		default:
			rprintfProgStrM("Found: No Partition!\r\n");
			//return 1;
			break;
	}

	// set current directory to root (\)
	CurrentDirStartCluster = RootDirStartCluster;
	PathNameBuffer[0] = '\\';
	PathNameBuffer[1] = 0;


	// do debug
#ifdef DEBUG_FAT
	switch (PartInfo.prPartType)
	{
		case PART_TYPE_DOSFAT16:
				rprintfProgStrM("Found: DOSFAT 16\r\n");
				break;
		case PART_TYPE_FAT16:
				rprintfProgStrM("Found: FAT16\r\n");
				break;
		case PART_TYPE_FAT16LBA:
				rprintfProgStrM("Found: FAT16 LBA\r\n");
				break;
		case PART_TYPE_FAT32LBA:
				rprintfProgStrM("Found: FAT32 LBA\r\n");
				break;
		case PART_TYPE_FAT32:
				rprintfProgStrM("Found: FAT32\r\n");
				//return 1;	
				break;
		default:
				rprintfProgStrM("Found: No Partition!\r\n");
				//return 1;
				break;
	}

	rprintfProgStrM("First sector    : ");	rprintfu32(PartInfo.prStartLBA);	rprintfCRLF();
	rprintfProgStrM("Size            : ");	rprintfu32(PartInfo.prSize);		rprintfCRLF();
	rprintfProgStrM("bytes/sector    : ");	rprintfu16(bpb->bpbBytesPerSec);	rprintfCRLF();
	rprintfProgStrM("sectors/cluster : ");	rprintfu08(bpb->bpbSecPerClust);	rprintfCRLF();
	rprintfProgStrM("reserved sectors: ");	rprintfu16(bpb->bpbResSectors);		rprintfCRLF();
	rprintfProgStrM("FatSectors      : ");	rprintfu16(bpb->bpbFATsecs);		rprintfCRLF();
	rprintfProgStrM("BigFatSectors   : ");	rprintfu32(bpb->bpbBigFATsecs);		rprintfCRLF();
	rprintfProgStrM("Number of Fats  : ");	rprintfu08(bpb->bpbFATs);			rprintfCRLF();
	rprintfProgStrM("First Fat Sector: ");	rprintfu32(FirstFATSector);			rprintfCRLF();
	rprintfProgStrM("First Data Sect : ");	rprintfu32(FirstDataSector);		rprintfCRLF();
	rprintfProgStrM("RootDirStartClus: ");	rprintfu32(RootDirStartCluster);	rprintfCRLF();
#endif

	return 0;	
}

//////////////////////////////////////////////////////////////

unsigned char fatGetDirEntry(unsigned short entry)
{
	unsigned long sector;
	struct direntry *de = 0;	// avoid compiler warning by initializing
	struct winentry *we;
	unsigned char haveLongNameEntry;
	unsigned char gotEntry;
	unsigned short b;
	int i,index;
	char *fnbPtr;
	unsigned short entrycount = 0;

	// read dir data
	sector = fatClustToSect(CurrentDirStartCluster);

	haveLongNameEntry = 0;
	gotEntry = 0;

	index = 16;	// crank it up
	
	//while(entrycount < entry) 
	while(1)
	{
		if(index == 16)	// time for next sector ?
		{
			ataReadSectors( DRIVE0, sector++, 1, SectorBuffer);
			de = (struct direntry *) SectorBuffer;
			index = 0;
		}
		
		// check the status of this directory entry slot
		if(de->deName[0] == 0x00)
		{
			// slot is empty and this is the end of directory
			gotEntry = 0;
			break;
		}
		else if(de->deName[0] == 0xE5)
		{
			// this is an empty slot
			// do nothing and move to the next one
		}
		else
		{
			// this is a valid and occupied entry
			// is it a part of a long file/dir name?
			if(de->deAttributes == ATTR_LONG_FILENAME)
			{
				// we have a long name entry
				// cast this directory entry as a "windows" (LFN: LongFileName) entry
				we = (struct winentry *) de;
				
				b = WIN_ENTRY_CHARS*( (we->weCnt-1) & 0x0f);		// index into string
				fnbPtr = &FileNameBuffer[b];
				for (i=0;i<5;i++)	*fnbPtr++ = we->wePart1[i*2];	// copy first part
				for (i=0;i<6;i++)	*fnbPtr++ = we->wePart2[i*2];	// second part
				for (i=0;i<2;i++)	*fnbPtr++ = we->wePart3[i*2];	// and third part
				if (we->weCnt & WIN_LAST) *fnbPtr = 0;				// in case dirnamelength is multiple of 13, add termination
				if ((we->weCnt & 0x0f) == 1) haveLongNameEntry = 1;	// flag that we have a complete long name entry set
			}
			else
			{
				// we have a short name entry
				
				// check if this is the short name entry corresponding
				// to the end of a multi-part long name entry
				if(haveLongNameEntry)
				{
					// a long entry name has been collected
					if(entrycount == entry)		
					{
						// desired entry has been found, break out
						gotEntry = 1;
						break;
					}
					// otherwise
					haveLongNameEntry = 0;	// clear long name flag
					entrycount++;			// increment entry counter		
				}
				else
				{
					// entry is a short name (8.3 format) without a
					// corresponding multi-part long name entry
					fnbPtr = FileNameBuffer;
					for (i=0;i<8;i++)	*fnbPtr++ = de->deName[i];		// copy name
					*fnbPtr++ = '.';									// insert '.'
					for (i=0;i<3;i++)	*fnbPtr++ = de->deExtension[i];	// copy extension
					*fnbPtr = 0;										// null-terminate

					if(entrycount == entry)		
					{
						// desired entry has been found, break out
						gotEntry = 1;
						break;
					}
					// otherwise
					entrycount++;			// increment entry counter		
				}
			}
		}
		// next directory entry
		de++;
		// next index
		index++;
	}
	
	// we have a file/dir to return
	// store file/dir starting cluster (start of data)
	FileInfo.StartCluster = (unsigned long) ((unsigned long)de->deHighClust << 16) + de->deStartCluster;
	// store file/dir size
	// (note: size field for subdirectory entries is always zero)
	FileInfo.Size = de->deFileSize;
	// store file/dir attributes
	FileInfo.Attr = de->deAttributes;
	// store file/dir creation time
	FileInfo.CreateTime = de->deCTime[0] | de->deCTime[1]<<8;
	// store file/dir creation date
	FileInfo.CreateTime = de->deCDate[0] | de->deCDate[1]<<8;

	return gotEntry;
}

// change directory into 
unsigned char fatChangeDirectory(unsigned short entry)
{
	// get the requested directory entry
	if(	fatGetDirEntry(entry) )
	{
		// make sure the entry is a directory
		if(FileInfo.Attr & ATTR_DIRECTORY)
		{
			// change directories into this directory
			// check to see if we are changing back to root directory
			if(FileInfo.StartCluster)
			{
				// standard change directory
				CurrentDirStartCluster = FileInfo.StartCluster;
			}
			else
			{
				// if startCluster pointer is zero,
				// a change to the root directory is intended
				// change directory to root
				CurrentDirStartCluster = RootDirStartCluster;
			}
			// TODO: handle pathname properly for going up a directory
			// set path string
			strcat(PathNameBuffer, FileNameBuffer);
			strcat(PathNameBuffer, "\\");
			// return success
			return TRUE;
		}
		else
		{
			// not a directory, cannot CD into a file!
			return FALSE;
		}
	}
	else
	{
		// not a valid entry, cannot CD!
		return FALSE;
	}
}

void fatPrintDirEntry(void)
{
	// print a formatted dir-style output for most recent file
	// print date
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateDate&DD_MONTH_MASK)>>DD_MONTH_SHIFT );	// month
	rprintfChar('/');
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateDate&DD_DAY_MASK)>>DD_DAY_SHIFT );		// day
	rprintfChar('/');
	rprintfNum(10, 4, FALSE, '0', (FileInfo.CreateDate&DD_YEAR_MASK)>>DD_YEAR_SHIFT );	// year
	rprintfChar(' ');

	// print time
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateTime&DT_HOURS_MASK)>>DT_HOURS_SHIFT );	// month
	rprintfChar(':');
	rprintfNum(10, 2, FALSE, '0', (FileInfo.CreateTime&DT_MINUTES_MASK)>>DT_MINUTES_SHIFT );		// day
	rprintfChar(':');
	rprintfNum(10, 2, FALSE, '0', 2*(FileInfo.CreateTime&DT_2SECONDS_MASK)>>DT_2SECONDS_SHIFT );	// seconds
	rprintfChar(' ');

	// print attributes
	if(FileInfo.Attr & ATTR_VOLUME)		rprintfChar('V'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_DIRECTORY)	rprintfChar('D'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_READONLY)	rprintfChar('R'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_HIDDEN)		rprintfChar('H'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_SYSTEM)		rprintfChar('S'); else rprintfChar('-');
	if(FileInfo.Attr & ATTR_ARCHIVE)	rprintfChar('A'); else rprintfChar('-');
	rprintfChar(' ');

	// print filesize
	rprintfNum(10, 8, FALSE, ' ', FileInfo.Size);	// filesize
	rprintfChar(' ');

	// print filename
	rprintfStr(FileNameBuffer);
}

void fatDumpDirSlot(unsigned short slot)
{
	unsigned long sector;
	// load correct sector
	sector = fatClustToSect(CurrentDirStartCluster);
	sector += slot/DIRENTRIES_PER_SECTOR;
	// print the entry as a hex table
	debugPrintHexTable(32, SectorBuffer+(slot<<5) );
}

struct FileInfoStruct* fatGetFileInfo(void)
{
	return &FileInfo;
}

// return the size of the last directory entry
unsigned long fatGetFilesize(void)
{
	return FileInfo.Size;
}

// return the long name of the last directory entry
char* fatGetFilename(void)
{	
	return FileNameBuffer;
}

// return the directory of the last directory entry
char* fatGetDirname(void)
{	
	return PathNameBuffer;
}

// load a clusterfull of data
void fatLoadCluster(unsigned long cluster, unsigned char *buffer)
{
	register unsigned char i;
	// read cluster
	//while ( ataReadSectors( DRIVE0, clust2sect(cluster), SectorsPerCluster, buffer) != 0);
	for(i=0; i<SectorsPerCluster; i++)
	{
		ataReadSectors( DRIVE0, fatClustToSect(cluster)+i, 1, buffer+(i<<9) );
		// temporary fix for wierd misaligned cluster problem
		// (only when using FAT16?)
//		ataReadSectors( DRIVE0, fatClustToSect(cluster+8)+i, 1, buffer+(i<<9) );
	}
}


// find next cluster in the FAT chain
unsigned long fatNextCluster(unsigned long cluster)
{
	unsigned long nextCluster;
	unsigned long fatMask;
	unsigned long fatOffset;
	unsigned long sector;
	unsigned int offset;
	
	// get fat offset in bytes
	if(Fat32Enabled)
	{
		// four FAT bytes (32 bits) for every cluster
		fatOffset = cluster << 2;
		// set the FAT bit mask
		fatMask = FAT32_MASK;
	}
	else
	{
		// two FAT bytes (16 bits) for every cluster
		fatOffset = cluster << 1;
		// set the FAT bit mask
		fatMask = FAT16_MASK;
	}
	
	// calculate the FAT sector that we're interested in
	sector = FirstFATSector + (fatOffset / BytesPerSector);
	// calculate offset of the our entry within that FAT sector
	offset = fatOffset % BytesPerSector;

	// if we don't already have this FAT chunk loaded, go get it
	if (sector != FatInCache)
	{
		// read sector of FAT table
		while (ataReadSectors( DRIVE0, sector, 1, (unsigned char*)FAT_CACHE_ADDR) != 0);
		FatInCache = sector;
	}

	// read the nextCluster value
	nextCluster = (*((unsigned long*) &((char*)FAT_CACHE_ADDR)[offset])) & fatMask;

	// check to see if we're at the end of the chain
	if (nextCluster == (CLUST_EOFE & fatMask))
		nextCluster = 0;

#ifdef DEBUG_FAT
	rprintfProgStrM(">");
	rprintfu32(nextCluster);
	rprintfCRLF();
#endif
	
	return nextCluster;
}

#endif
