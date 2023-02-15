namespace MauiCalculator;

public partial class ProgressDemo : ContentPage
{
    public ProgressDemo()
    {
        InitializeComponent();
    }

    private Task<double> _calcSinTask;
    private CancellationTokenSource _cts = null;
    private double _step = 0.000000002;

    private double CalculateSin()
    {
        MainThread.BeginInvokeOnMainThread(() =>
        {
            output.Text = "Вычисление";
        });
        double border = 0;
        double sin = 0;
        int percent = 0;
        int prevPercent = -1;
        while (border <= 1)
        {
            if (_cts.IsCancellationRequested)
            {
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    output.Text = "Задание отменено";
                });
                return sin;
            }
            sin += Math.Sin(border) * _step;
            border += _step;
            percent = (int)(border / 1.0 * 100);

            if (percent != prevPercent)
            {
                prevPercent = percent;
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    progressBar.Progress = percent / 100.0;
                    percentLbl.Text = percent + "%";
                });
            }
        }
        return sin;
    }
    private async void OnStartClicked(object sender, EventArgs e)
    {
        if (_calcSinTask?.Status != TaskStatus.Running)
        {
            _cts = new CancellationTokenSource();
            _calcSinTask = new Task<double>(CalculateSin);
            _calcSinTask.Start();
            double sin = await _calcSinTask;
            output.Text = "Значение синуса = "+sin.ToString();
            _cts = null;
        }
    }

    private void OnCancelClicked(object sender, EventArgs e)
    {
        if (_calcSinTask?.Status == TaskStatus.Running)
        {
            _cts.Cancel();
        }
    }
}