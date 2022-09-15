using System;
using System.Collections.Generic;

namespace Task;

class Journal
{
    private List<string> _registeredEvents = new();

    public void RecordChangesInOrdersOrClientsList(object? sender, CompanyEvents.CompanyEventArgs e)
    {
        {
            _registeredEvents.Add($"Event #{_registeredEvents.Count + 1}:{Environment.NewLine}Message: {e.Message} From {sender?.GetType().Name}\n");
        }
    }

    public void PrintRegisteredEvents()
    {
        foreach (string eventMessage in _registeredEvents)
        {
            Console.WriteLine(eventMessage);
        }
    }
}

