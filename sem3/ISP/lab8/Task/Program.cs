using System.Diagnostics;
using TaskLib;

namespace TaskSpace
{
    public class Program
    {
        private static Progress<string> progress = new();
        private static Random random = new();
        static async Task Main(string[] args)
        {
            Stopwatch stopwatch = new();
            stopwatch.Start();
            progress.ProgressChanged += (_, eArgs) =>
            {
                Console.WriteLine(eArgs);
            };
            Car[] carDealership = new Car[1000];
            for (int i = 0; i < 1000; i++)
            {
                carDealership[i] = new Car()
                {
                    ID = i,
                    Name = $"Car_{i}",
                    EngineCapacity = random.Next(1, 9)
                };
            }

            Console.WriteLine($"Thread {Thread.CurrentThread.ManagedThreadId} has started its work");
            StreamService<Car> streamService = new();
            using (MemoryStream memoryStream = new())
            {
                string filename = "CarDealership";

                var task1 = streamService.WriteToStreamAsync(memoryStream, carDealership, progress);
                await Task.Delay(200);
                var task2 = streamService.CopyFromStreamAsync(memoryStream, filename, progress);

                Console.WriteLine($"Thread {Thread.CurrentThread.ManagedThreadId}. Methods 1 and 2 are working.");
                Task.WaitAll(task2, task1);

                var task3 = streamService.GetStatisticsAsync(filename, obj => obj.EngineCapacity > 2);
                Console.WriteLine("Waiting for completion of calculations...");
                await task3;
                Console.WriteLine($"Number of cars with engine capacity > 2 equals {task3.Result}");
            }
            stopwatch.Stop();
            TimeSpan ts = stopwatch.Elapsed;
            string elapsedTime = string.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                    ts.Hours, ts.Minutes, ts.Seconds,
                    ts.Milliseconds / 10);
            Console.WriteLine($"\nElapsed time {elapsedTime}");
        }
    }
}