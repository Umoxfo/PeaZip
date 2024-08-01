using System.ComponentModel;

namespace PeaZip.Config.ViewModels;

public partial class ContextMenuViewModel
{
    private static class PropertyChangingArgs
    {
        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractMain" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractMain = new(nameof(ContextMenuViewModel.ExtractMain));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractHere" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractHere = new(nameof(ContextMenuViewModel.ExtractHere));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractSmart" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractSmart = new(nameof(ContextMenuViewModel.ExtractSmart));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractFolder" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractFolder = new(nameof(ContextMenuViewModel.ExtractFolder));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractDesktop" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractDesktop = new(nameof(ContextMenuViewModel.ExtractDesktop));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractDocument" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractDocument = new(nameof(ContextMenuViewModel.ExtractDocument));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractDownload" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractDownload = new(nameof(ContextMenuViewModel.ExtractDownload));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "ExtractTest" generated properties.</summary>
        public static readonly PropertyChangingEventArgs ExtractTest = new(nameof(ContextMenuViewModel.ExtractTest));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "BrowseArchive" generated properties.</summary>
        public static readonly PropertyChangingEventArgs BrowseArchive = new(nameof(ContextMenuViewModel.BrowseArchive));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "BrowsePath" generated properties.</summary>
        public static readonly PropertyChangingEventArgs BrowsePath = new(nameof(ContextMenuViewModel.BrowsePath));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddArchive" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddArchive = new(nameof(ContextMenuViewModel.AddArchive));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddSeparateArchive" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddSeparateArchive = new(nameof(ContextMenuViewModel.AddSeparateArchive));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddSfx" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddSfx = new(nameof(ContextMenuViewModel.AddSfx));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "Add7z" generated properties.</summary>
        public static readonly PropertyChangingEventArgs Add7z = new(nameof(ContextMenuViewModel.Add7z));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddBrotli" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddBrotli = new(nameof(ContextMenuViewModel.AddBrotli));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddBZip2" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddBZip2 = new(nameof(ContextMenuViewModel.AddBZip2));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddGz" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddGz = new(nameof(ContextMenuViewModel.AddGz));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddPea" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddPea = new(nameof(ContextMenuViewModel.AddPea));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddTar" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddTar = new(nameof(ContextMenuViewModel.AddTar));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddWim" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddWim = new(nameof(ContextMenuViewModel.AddWim));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddXz" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddXz = new(nameof(ContextMenuViewModel.AddXz));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddZip" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddZip = new(nameof(ContextMenuViewModel.AddZip));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddZqap" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddZqap = new(nameof(ContextMenuViewModel.AddZqap));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddZstd" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddZstd = new(nameof(ContextMenuViewModel.AddZstd));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "Add7zEncrypt" generated properties.</summary>
        public static readonly PropertyChangingEventArgs Add7zEncrypt = new(nameof(ContextMenuViewModel.Add7zEncrypt));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "AddZipMail" generated properties.</summary>
        public static readonly PropertyChangingEventArgs AddZipMail = new(nameof(ContextMenuViewModel.AddZipMail));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "Split" generated properties.</summary>
        public static readonly PropertyChangingEventArgs Split = new(nameof(ContextMenuViewModel.Split));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "Convert" generated properties.</summary>
        public static readonly PropertyChangingEventArgs Convert = new(nameof(ContextMenuViewModel.Convert));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "Analyze" generated properties.</summary>
        public static readonly PropertyChangingEventArgs Analyze = new(nameof(ContextMenuViewModel.Analyze));

        /// <summary>The cached <see cref="PropertyChangingEventArgs"/> instance for all "Wipe" generated properties.</summary>
        public static readonly PropertyChangingEventArgs Wipe = new(nameof(ContextMenuViewModel.Wipe));
    }

    private static class PropertyChangedArgs
    {
        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractMain" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractMain = new(nameof(ContextMenuViewModel.ExtractMain));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractHere" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractHere = new(nameof(ContextMenuViewModel.ExtractHere));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractSmart" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractSmart = new(nameof(ContextMenuViewModel.ExtractSmart));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractFolder" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractFolder = new(nameof(ContextMenuViewModel.ExtractFolder));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractDesktop" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractDesktop = new(nameof(ContextMenuViewModel.ExtractDesktop));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractDocument" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractDocument = new(nameof(ContextMenuViewModel.ExtractDocument));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractDownload" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractDownload = new(nameof(ContextMenuViewModel.ExtractDownload));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "ExtractTest" generated properties.</summary>
        public static readonly PropertyChangedEventArgs ExtractTest = new(nameof(ContextMenuViewModel.ExtractTest));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "BrowseArchive" generated properties.</summary>
        public static readonly PropertyChangedEventArgs BrowseArchive = new(nameof(ContextMenuViewModel.BrowseArchive));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "BrowsePath" generated properties.</summary>
        public static readonly PropertyChangedEventArgs BrowsePath = new(nameof(ContextMenuViewModel.BrowsePath));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddArchive" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddArchive = new(nameof(ContextMenuViewModel.AddArchive));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddSeparateArchive" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddSeparateArchive = new(nameof(ContextMenuViewModel.AddSeparateArchive));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddSfx" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddSfx = new(nameof(ContextMenuViewModel.AddSfx));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "Add7z" generated properties.</summary>
        public static readonly PropertyChangedEventArgs Add7z = new(nameof(ContextMenuViewModel.Add7z));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddBrotli" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddBrotli = new(nameof(ContextMenuViewModel.AddBrotli));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddBZip2" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddBZip2 = new(nameof(ContextMenuViewModel.AddBZip2));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddGz" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddGz = new(nameof(ContextMenuViewModel.AddGz));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddPea" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddPea = new(nameof(ContextMenuViewModel.AddPea));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddTar" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddTar = new(nameof(ContextMenuViewModel.AddTar));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddWim" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddWim = new(nameof(ContextMenuViewModel.AddWim));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddXz" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddXz = new(nameof(ContextMenuViewModel.AddXz));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddZip" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddZip = new(nameof(ContextMenuViewModel.AddZip));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddZqap" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddZqap = new(nameof(ContextMenuViewModel.AddZqap));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddZstd" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddZstd = new(nameof(ContextMenuViewModel.AddZstd));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "Add7zEncrypt" generated properties.</summary>
        public static readonly PropertyChangedEventArgs Add7zEncrypt = new(nameof(ContextMenuViewModel.Add7zEncrypt));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "AddZipMail" generated properties.</summary>
        public static readonly PropertyChangedEventArgs AddZipMail = new(nameof(ContextMenuViewModel.AddZipMail));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "Split" generated properties.</summary>
        public static readonly PropertyChangedEventArgs Split = new(nameof(ContextMenuViewModel.Split));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "Convert" generated properties.</summary>
        public static readonly PropertyChangedEventArgs Convert = new(nameof(ContextMenuViewModel.Convert));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "Analyze" generated properties.</summary>
        public static readonly PropertyChangedEventArgs Analyze = new(nameof(ContextMenuViewModel.Analyze));

        /// <summary>The cached <see cref="PropertyChangedEventArgs"/> instance for all "Wipe" generated properties.</summary>
        public static readonly PropertyChangedEventArgs Wipe = new(nameof(ContextMenuViewModel.Wipe));
    }
}
