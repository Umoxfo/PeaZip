using System.Runtime.CompilerServices;

using Microsoft.Extensions.Primitives;
using Microsoft.Win32;

using PeaZip.ShellEx.Flags;

using Windows.Storage;

namespace PeaZip.Config.ViewModels;

public partial class ContextMenuViewModel
{
    [MethodImpl(MethodImplOptions.AggressiveOptimization)]
    private void LoadContextMenuConfig()
    {
        if (localSettings.Values["contextMenu"] is ApplicationDataCompositeValue composite)
        {
            extract = (Extract)composite["extract"];
            browse = (Browse)composite["browse"];
            archiveFormat = (ArchiveFormat)composite["archiveFormat"];
            archiveFormatFeature = (ArchiveFormatFeature)composite["archiveFormatFeature"];
            archiveFeature = (ArchiveFeature)composite["archiveFeature"];
            utils = (Utils)composite["utils"];
        }
        else
        {
            LoadConfigFromRegistry();
        }
    }

    private void LoadConfigFromRegistry()
    {
        using var subKey = Registry.ClassesRoot.OpenSubKey(@"AllFilesystemObjects\shell\PeaZip");
        if (subKey?.GetValue("SubCommands") is string subCommands)
        {
            var tokenizer = new StringTokenizer(subCommands, [';']);
            foreach (var segment in tokenizer)
            {
                var pageId = segment.Trim();
                if (StringSegment.IsNullOrEmpty(pageId))
                {
                    continue;
                }

                switch (pageId.Value)
                {
                    case "PeaZip.ext2main":
                        extract |= Extract.ExtractMain;
                        break;
                    case "PeaZip.ext2here":
                        extract |= Extract.ExtractHere;
                        break;
                    case "PeaZip.ext2smart":
                        extract |= Extract.ExtractSmart;
                        break;
                    case "PeaZip.ext2folder":
                        extract |= Extract.ExtractFolder;
                        break;
                    case "PeaZip.ext2test":
                        extract |= Extract.ExtractTest;
                        break;

                    case "PeaZip.ext2browseasarchive":
                        browse |= Browse.BrowseArchive;
                        break;
                    case "PeaZip.ext2browsepath":
                        browse |= Browse.BrowsePath;
                        break;

                    case "PeaZip.add2separate":
                        archiveFormat |= ArchiveFormat.AddArchive;
                        break;
                    case "PeaZip.add2separatesingle":
                        archiveFormat |= ArchiveFormat.AddSeparateArchive;
                        break;
                    case "PeaZip.add2separatesfx":
                        archiveFormat |= ArchiveFormat.AddSfx;
                        break;
                    case "PeaZip.add2separate7z":
                        archiveFormat |= ArchiveFormat.Add7z;
                        break;
                    case "PeaZip.add2separategz":
                        archiveFormat |= ArchiveFormat.AddGz;
                        break;
                    case "PeaZip.add2separatexz":
                        archiveFormat |= ArchiveFormat.AddXz;
                        break;
                    case "PeaZip.add2separatezip":
                        archiveFormat |= ArchiveFormat.AddZip;
                        break;
                    case "PeaZip.add2separatezstd":
                        archiveFormat |= ArchiveFormat.AddZstd;
                        break;

                    case "PeaZip.add2separate7zencrypt":
                        archiveFormatFeature |= ArchiveFormatFeature.Add7zEncrypt;
                        break;

                    case "PeaZip.add2separatezipmail":
                        archiveFeature |= ArchiveFeature.AddZipMail;
                        break;
                    case "PeaZip.add2split":
                        archiveFeature |= ArchiveFeature.Split;
                        break;
                    case "PeaZip.add2convert":
                        archiveFeature |= ArchiveFeature.Convert;
                        break;

                    case "PeaZip.analyze":
                        utils |= Utils.Analyze;
                        break;
                    case "PeaZip.add2wipe":
                        utils |= Utils.Wipe;
                        break;

                    default:
                        break;
                }
            }
        }
        else
        {
            DefaultContextMenuItems();
        }
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining | MethodImplOptions.AggressiveOptimization)]
    private void DefaultContextMenuItems()
    {
        extract = Extract.ExtractMain | Extract.ExtractHere | Extract.ExtractSmart | Extract.ExtractFolder;
        browse = Browse.BrowseArchive | Browse.BrowsePath;
        archiveFormat = ArchiveFormat.AddArchive | ArchiveFormat.Add7z | ArchiveFormat.AddZip;
        archiveFormatFeature = ArchiveFormatFeature.Add7zEncrypt;
        archiveFeature = ArchiveFeature.Convert;
        utils = Utils.Analyze;
    }

    [MethodImpl(MethodImplOptions.AggressiveOptimization)]
    private void StoreContextMenuConfig()
    {
        localSettings.Values["contextMenu"] = new ApplicationDataCompositeValue
        {
            ["extract"] = (uint)extract,
            ["browse"] = (uint)browse,
            ["archiveFormat"] = (uint)archiveFormat,
            ["archiveFormatFeature"] = (uint)archiveFormatFeature,
            ["archiveFeature"] = (uint)archiveFeature,
            ["utils"] = (uint)utils,
        };
    }
}
