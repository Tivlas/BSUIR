using System;
using Task;

class Program
{
    static void Main(string[] args)
    {
        Test();
        AddRemoveTest();
    }

    public static void Test()
    {
        var company = new FreightShippingCompany();

        company.AddOrder(new Order(100, "Moscow - Saint-Petersburg"));
        company.AddOrder(new Order(200, "Minsk - Gomel"));
        company.AddOrder(new Order(300, "Minsk - Moscow"));
        company.AddOrder(new Order(400, "Minsk - Saint-Petersburg"));
        company.AddOrder(new Order(500, "Grodno - Brest"));

        company.RegisterClient(new Client("1234567890"));
        company.RegisterClient(new Client("0987654321"));
        company.RegisterClient(new Client("1111111111"));
        company.RegisterClient(new Client("2222222222"));
        company.RegisterClient(new Client("3333333333"));
        company.RegisterClient(new Client("4444444444"));

        try
        {
            company.PlaceOrderToClient(new Order(100, "Moscow - Saint-Petersburg"), "1234567890");
            company.PlaceOrderToClient(new Order(200, "Minsk - Gomel"), "1234567890");
            //company.PlaceOrderToClient(new Order(300, "Minsk - Moscow"), "0234567890");
            //.PlaceOrderToClient(new Order(400, "Vitebsk - Saint-Petersburg"), "1234567890");
            company.PlaceOrderToClient(new Order(500, "Grodno - Brest"), "1111111111");
            Console.WriteLine($"Total cost for 1234567890 {company.CalculateTotalOrdersCostForClient("1234567890")}");
            Console.WriteLine($"Total cost for 1111111111 {company.CalculateTotalOrdersCostForClient("1111111111")}");
            Console.WriteLine($"Total cost for all clients {company.CalculateTotalOrdersCostForAllClients()}");
            company.RemoveClient(new Client("1234567890"));
            //company.PlaceOrderToClient(new Order(500, "Grodno - Brest"), "1234567890");
            company.RemoveOrder(new Order(500, "Grodno - Brest"));
            company.PlaceOrderToClient(new Order(500, "Grodno - Brest"), "4444444444");
        }
        catch (CustomExceptions.OrderNotFoundException e)
        {
            Console.WriteLine(e.Message);
        }
        catch (CustomExceptions.ClientNotFoundException e)
        {
            Console.WriteLine(e.Message);
        }
    }

    public static void AddRemoveTest()
    {
        FreightShippingCompany company = new();
        company.RegisterClient(new Client("1234567890"));
        company.RegisterClient(new Client("0987654321"));
        company.RegisterClient(new Client("1111111111"));
        company.RegisterClient(new Client("2222222222"));
        company.RemoveClient(new Client("2222222222"));
        company.RegisterClient(new Client("3333333333"));
        company.RegisterClient(new Client("4444444444"));

        company.RemoveClient(new Client("1234567890"));
        company.RemoveClient(new Client("0987654321"));
        company.RemoveClient(new Client("1111111111"));
        company.RemoveClient(new Client("3333333333"));
        company.RemoveClient(new Client("4444444444"));

        company.RegisterClient(new Client("1234567890"));
        Console.WriteLine($"Total cost for 1234567890 {company.CalculateTotalOrdersCostForClient("1234567890")}");

        company.AddOrder(new Order(100, "Moscow - Saint-Petersburg"));

    }
}


