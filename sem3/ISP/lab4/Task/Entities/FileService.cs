namespace Task
{
    public class FileService : IFileService<StoreGood>
    {
        public IEnumerable<StoreGood> ReadFile(string fileName)
        {
            using (BinaryReader br = new BinaryReader(File.OpenRead(fileName)))
            {
                while (br.PeekChar() > -1)
                {
                    yield return new StoreGood
                    {
                        Name = br.ReadString(),
                        Cost = br.ReadInt32(),
                        IsAtDiscount = br.ReadBoolean()
                    };
                }
            }
        }

        public void SaveData(IEnumerable<StoreGood> data, string fileName)
        {
            if (File.Exists(fileName))
            {
                File.Delete(fileName);
                File.Create("savedCollectionInfo.bin").Close();
            }
            using (BinaryWriter bw = new BinaryWriter(File.OpenWrite(fileName)))
            {
                foreach (var item in data)
                {
                    bw.Write(item.Name);
                    bw.Write(item.Cost);
                    bw.Write(item.IsAtDiscount);
                }
            }
        }
    }
}
