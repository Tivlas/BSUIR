namespace Task;

public class Order
{
    public int Cost { get; private set; }
    public string Route { get; private set; }

    public Order(int cost, string route)
    {
        Cost = cost;
        Route = route;
    }

    public override bool Equals(object? obj)
    {
        if (obj == null || GetType() != obj.GetType())
        {
            return false;
        }
        var order = (Order)obj;
        return order.Cost == Cost && order.Route == Route;
    }

    public override int GetHashCode()
    {
        return Route.GetHashCode();
    }

    public override string ToString()
    {
        return $"Route: {Route}, Cost: {Cost}";
    }
}