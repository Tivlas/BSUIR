using System.Text.Json;
using task;


namespace FileServiceLib
{
    public class FileService<T> : IFileService<T> where T : class
    {
        public IEnumerable<T> ReadFile(string fileName)
        {
            var json = File.ReadAllText(fileName);
            var data = JsonSerializer.Deserialize<IEnumerable<T>>(json);
            return data;
        }

        public void SaveData(IEnumerable<T> data, string fileName)
        {
            var options = new JsonSerializerOptions
            {
                WriteIndented = true
            };
            var json = JsonSerializer.Serialize(data, options);

            using (var file = File.CreateText(fileName))
            {
                file.Write(json);
            }
        }
    }
}
