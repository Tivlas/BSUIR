using System.Text.Json;

namespace TaskLib
{
    public class StreamService<T>
    {
        private Semaphore sem = new(1, 1);
        public async Task WriteToStreamAsync(Stream stream, IEnumerable<T> data, IProgress<string> progress)
        {
            sem.WaitOne();
            progress.Report($"\nStarted writing to stream in thread {Thread.CurrentThread.ManagedThreadId}");
            JsonSerializerOptions options = new()
            {
                WriteIndented = true
            };
            await JsonSerializer.SerializeAsync(stream, data, options);
            await VisualizeProgress(progress, "writting");
            progress.Report($"Finished writing to stream in thread {Thread.CurrentThread.ManagedThreadId}\n");
            sem.Release();
        }

        public async Task CopyFromStreamAsync(Stream stream, string filename, IProgress<string> progress)
        {
            sem.WaitOne();
            progress.Report($"\nStarted copying from stream in thread {Thread.CurrentThread.ManagedThreadId}");
            using (var fileStream = File.Create(filename))
            {
                stream.Position = 0;
                await stream.CopyToAsync(fileStream);
            }
            await VisualizeProgress(progress, "copying");
            progress.Report($"Finished copying from stream in thread {Thread.CurrentThread.ManagedThreadId}\n");
            sem.Release();
        }

        public async Task<int?> GetStatisticsAsync(string filename, Func<T, bool> filter)
        {
            using (var stream = File.OpenRead(filename))
            {
                IEnumerable<T>? data = await JsonSerializer.DeserializeAsync<IEnumerable<T>>(stream);
                return data?.AsParallel().Where(filter).Count();
            }
        }

        private async Task VisualizeProgress(IProgress<string> progress, string performedAction)
        {
            for (int i = 0; i <= 100; i += 20)
            {
                await Task.Delay(500);
                progress?.Report(new string($"Current progress ({performedAction}) in thread {Thread.CurrentThread.ManagedThreadId,2} is: {i,3}%"));
            }
        }
    }
}
