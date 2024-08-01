namespace PeaZip.ShellEx.Flags;

[Flags]
public enum ArchiveFormat : uint
{
    None = 0,
    AddArchive = 0b1,
    AddSeparateArchive = AddArchive << 1,
    AddSfx = AddSeparateArchive << 1,
    Add7z = AddSfx << 1,
    AddBrotli = Add7z << 1,
    AddBZip2 = AddBrotli << 1,
    AddGz = AddBZip2 << 1,
    AddPea = AddGz << 1,
    AddTar = AddPea << 1,
    AddWim = AddTar << 1,
    AddXz = AddWim << 1,
    AddZip = AddXz << 1,
    AddZqap = AddZip << 1,
    AddZstd = AddZqap << 1,
}
