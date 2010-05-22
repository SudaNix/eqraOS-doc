//-- header information format for PE files -------------------

typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
    ushort e_magic;         // Magic number (Should be MZ
    ushort e_cblp;          // Bytes on last page of file
    ushort e_cp;            // Pages in file
    ushort e_crlc;          // Relocations
    ushort e_cparhdr;       // Size of header in paragraphs
    ushort e_minalloc;      // Minimum extra paragraphs needed
    ushort e_maxalloc;      // Maximum extra paragraphs needed
    ushort e_ss;            // Initial (relative) SS value
    ushort e_sp;            // Initial SP value
    ushort e_csum;          // Checksum
    ushort e_ip;            // Initial IP value
    ushort e_cs;            // Initial (relative) CS value
    ushort e_lfarlc;        // File address of relocation table
    ushort e_ovno;          // Overlay number
    ushort e_res[4];        // Reserved words
    ushort e_oemid;         // OEM identifier (for e_oeminfo)
    ushort e_oeminfo;       // OEM information; e_oemid specific
    ushort e_res2[10];      // Reserved words
    long   e_lfanew;        // File address of new exe header
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

//------ Real mode stub program --------
//------ Here is the file signiture, such as PE00 for NT ---

typedef struct _IMAGE_FILE_HEADER {
    ushort  Machine;
    ushort  NumberOfSections;
    ulong   TimeDateStamp;
    ulong   PointerToSymbolTable;
    ulong   NumberOfSymbols;
    ushort  SizeOfOptionalHeader;
    ushort  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

struct _IMAGE_OPTIONAL_HEADER {
    //
    // Standard fields.
    //
    ushort  Magic;
    uchar   MajorLinkerVersion;
    uchar   MinorLinkerVersion;
    ulong   SizeOfCode;
    ulong   SizeOfInitializedData;
    ulong   SizeOfUninitializedData;
    ulong   AddressOfEntryPoint;			<< IMPORTANT!
    ulong   BaseOfCode;
    ulong   BaseOfData;
    //
    // NT additional fields.
    //
    ulong   ImageBase;
    ulong   SectionAlignment;
    ulong   FileAlignment;
    ushort  MajorOperatingSystemVersion;
    ushort  MinorOperatingSystemVersion;
    ushort  MajorImageVersion;
    ushort  MinorImageVersion;
    ushort  MajorSubsystemVersion;
    ushort  MinorSubsystemVersion;
    ulong   Reserved1;
    ulong   SizeOfImage;
    ulong   SizeOfHeaders;
    ulong   CheckSum;
    ushort  Subsystem;
    ushort  DllCharacteristics;
    ulong   SizeOfStackReserve;
    ulong   SizeOfStackCommit;
    ulong   SizeOfHeapReserve;
    ulong   SizeOfHeapCommit;
    ulong   LoaderFlags;
    ulong   NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;
