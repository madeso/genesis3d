#pragma once

enum class FileTypeIdentifier
{
	DOS = 1L,
	MEMORY = 2L,
	VIRTUAL = 3L
};

enum class FileAttributes
{
	READONLY = 0x00000001,
	DIRECTORY = 0x00000002,
};

struct FileHints
{
	void *HintData;
	int HintDataLength;
};

struct FileTime
{
	unsigned long Time1;
	unsigned long Time2;
};

struct FileProperties
{
	FileTime Time;
	FileAttributes AttributeFlags;
	long Size;
	FileHints Hints;
	char Name[1024];
};

/// First three flags are mutually exclusive.  Combining them will result in failure
/// returns for both OpenNewSystem and Open.
enum class OpenModeFlags
{
	READONLY = 0x00000001,
	UPDATE = 0x00000002,
	CREATE = 0x00000004,
	DIRECTORY = 0x00000008
};

enum class Seek
{
	CUR = 0,
	END = 1,
	SET = 2
};

/// Virtual file interface
struct File
{
	/// Returns the outer context in which file was opened.
	FileSystem *GetContext() const;

	/// closes and destroys the File
	bool Close();

	bool GetS(void *Buff, int MaxLen);
	bool Read(void *Buff, int Count);
	bool Write(const void *Buff, int Count);
	bool Seek(int where, Seek Whence);
	bool Printf(const char *Format, ...);
	bool EOF() const;
	bool Tell(long *Position) const;
	bool GetProperties(FileProperties *Properties) const;

	bool Size(long *Size) const;
	bool SetSize(long Size);
	bool SetAttributes(FileAttributes Attributes);
	bool SetTime(const FileTime *Time);
	bool SetHints(const FileHints *Hints);
};

struct FileSystem
{
	/// Creates a finder object from which you can get iterated file names.
	/// This is findfirst/findnext functionality.
	FileFinder *CreateFinder(const char *FileSpec);

	/// Opens a file / file system.
	FileSystem *OpenNewSystem(FileTypeIdentifier FileSystemType, const char *Name, void *Context, OpenModeFlags openModeFlags);

	bool UpdateContext(void *Context, int ContextSize);

	/// Returns the outer context in which file was opened.
	FileSystem *GetContext() const;

	File *Open(const char *Name, OpenModeFlags openModeFlags);

	/// Appends (or prepends) the path associated with FS2 into this.
	///    Append==true   causes the FS2 to be searched AFTER this
	///    Append==false  causes the FS2 to be searched BEFORE this
	bool AddPath(const File *FS2, bool Append);

	/// Deletes a file within a file system.  Returns true on success, false on failure.
	bool DeleteFile(const char *FileName);

	/// Renames a file within a file system.  Returns true on success, false on failure.
	bool RenameFile(const char *FileName, const char *NewName);

	/// Returns true if the file FileName exists in FS, false otherwise. Does not do any searching (?)
	bool FileExists(const char *FileName);

	/// closes and destroys the File
	bool Close();
};

// Finder (Directory)
struct FileFinder
{
	/// Destroys a Finder object
	void Destroy();

	/// Tracks to the next file in the finder directory
	bool GetNextFile();

	/// Gets the file properties from a FileFinder.  You cannot set properties for
	/// a file through a finder.  You have to set the properties through a File.
	bool GetProperties(FileProperties *Properties) const;
};
