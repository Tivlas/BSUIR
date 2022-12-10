using SinLibSpace;
using System.Text;
namespace Task
{
    internal class Program
    {
        private static object _lock = new();
        static void Main(string[] args)
        {
            Console.Clear();
            IntSin obj = new();
            StringBuilder progress = new();
            int size = 10;
            obj.SinCalculated += (_, eventArgs) =>
            {
                lock (_lock)
                {
                    int top = int.Parse(Thread.CurrentThread.Name!);
                    Console.SetCursorPosition(0, size + top);
                    Console.WriteLine($"Sin equals: {eventArgs.Sin:f4}. Elapsed time: {eventArgs.ElapsedTime}. Thread ID = {eventArgs.ThreadID,2}/Name = {eventArgs.ThreadName,-2} has finished it's work.\n"); 
                }
            };

            obj.ProgressEvent += (_, eventArgs) =>
            {
                lock (_lock)
                {
                    int percent = eventArgs.Percent;
                    int top = int.Parse(Thread.CurrentThread.Name!);
                    StringBuilder progress = new StringBuilder("[");
                    for (int i = 0; i < 100; i++)
                    {
                        if (i < percent)
                        {
                            progress.Append("=");
                        }
                        else if (i == percent)
                        {
                            progress.Append(">");
                        }
                        else
                        {
                            progress.Append(" ");
                        }
                    }
                    progress.Append("]");
                    progress.Append(percent.ToString() + '%');
                    Console.SetCursorPosition(0, top);
                    Console.WriteLine($"Thread {Thread.CurrentThread.ManagedThreadId,-2} with {Thread.CurrentThread.Priority,7} priority is working: {progress}"); 
                }
            };

            Thread[] threads = new Thread[size];
            for (int i = 0; i < size; i++)
            {
                ThreadStart start = obj.CalcSinIntegral;
                start += () =>
                {
                    lock (_lock)
                    {
                        Console.SetCursorPosition(0, size * 2 + 1); 
                    }
                };
                threads[i] = new Thread(start) { Name = (i + 1).ToString(), Priority = ThreadPriority.Normal };
            }
            threads[0].Priority = ThreadPriority.Highest;
            threads[^1].Priority = ThreadPriority.Lowest;
            foreach (Thread th in threads)
            {
                th.Start();
            }
        }
    }
}