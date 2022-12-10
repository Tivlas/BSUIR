using System.Diagnostics;

namespace SinLibSpace
{
    public class IntSin
    {
        private object _calcLock = new object();
        public event EventHandler<IntSinEventArgs>? SinCalculated;
        public event EventHandler<ProgressEventArgs>? ProgressEvent;
        private double _step = 0.000000001;
        private Semaphore _semaphore = new(4, 10);
        public void CalcSinIntegral()
        {
            //lock (_calcLock)
            {
                _semaphore.WaitOne();

                Thread cur = Thread.CurrentThread;
                double border = 0;
                double sin = 0;

                Stopwatch stopWatch = new Stopwatch();
                stopWatch.Start();
                int percent = 0;
                int prevPercent = -1;
                while (border <= 1)
                {
                    sin += Math.Sin(border) * _step;
                    border += _step;
                    percent = (int)(border / 1.0 * 100);

                   

                    if (percent != prevPercent)
                    {
                        ProgressEvent?.Invoke(this, new ProgressEventArgs(percent));
                        prevPercent = percent;
                    }
                }
                stopWatch.Stop();
                TimeSpan ts = stopWatch.Elapsed;
                string elapsedTime = string.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                    ts.Hours, ts.Minutes, ts.Seconds,
                    ts.Milliseconds / 10);
                SinCalculated?.Invoke(this, new IntSinEventArgs(sin, elapsedTime, cur.ManagedThreadId,cur.Name!));
                _semaphore.Release();
            }
        }
    }

    public class IntSinEventArgs : EventArgs
    {
        public readonly double Sin;
        public readonly string ElapsedTime;
        public int ThreadID;
        public string ThreadName;
        public IntSinEventArgs(double _sin, string _elapsedTime, int id, string name = "")
        {
            Sin = _sin;
            ElapsedTime = _elapsedTime;
            ThreadID = id;
            ThreadName = name;
        }
    }

    public class ProgressEventArgs : EventArgs
    {
        public readonly int Percent;
        public ProgressEventArgs(int percent)
        {
            Percent = percent;
        }
    }
}
