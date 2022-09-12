using System.Collections;

namespace Task;

public class Client 
{
    private MyCustomCollection<Order> _placedOrders = new MyCustomCollection<Order>();

    public string Name { get; set; } = "Just a client";
    public string ID { get; private set; } = "0000000000";


    public Client(string id)
    {
        ID = id;
    }

    public Client(string name, string id) : this(id)
    {
        Name = name;
    }

    public int CalculateTotalOrdersCost()
    {
        int totalCost = 0;
        _placedOrders.Reset();
        while (_placedOrders.Current() != null)
        {
            totalCost += _placedOrders.Current()!.Cost;
            _placedOrders.Next();
        }
        return totalCost;
    }

    public void PlaceOrder(Order order)
    {
        _placedOrders.Add(order);
    }

    public void RemoveOrder(Order order)
    {
        _placedOrders.Remove(order);
    }

    public override bool Equals(object? obj)
    {
        if (obj == null || GetType() != obj.GetType())
        {
            return false;
        }
        var client = (Client)obj;
        return client.ID == ID;
    }

    public override int GetHashCode()
    {
        return ID.GetHashCode();
    }
}