using System.Collections.Generic;
using System.Linq;

namespace Task;
public class Client
{
    private List<Order> _placedOrders = new();

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
        return (from order in _placedOrders select order.Cost).Sum();
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

    public override string ToString()
    {
        return $"Name: {Name}, ID: {ID}";
    }

    public Dictionary<string,int> GetListOfAmountsPaidForEachOrder(){
        return _placedOrders.GroupBy(order => order.Route).ToDictionary(group => group.Key, group => group.Sum(order => order.Cost));
    }
}