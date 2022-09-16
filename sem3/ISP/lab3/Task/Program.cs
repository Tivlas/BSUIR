using System;
using System.Collections.Generic;
using Task;

FreightShippingCompany company = new();
Journal journal = new();
company.OrdersOrClientsListChanged += journal.RecordChangesInOrdersOrClientsList;
company.ClientPlacedAnOrder += (sender, e) => Console.WriteLine($"Message: {e.Message}. From {sender?.GetType().Name}\n");

company.AddOrder(new Order(100, "Moscow - Saint-Petersburg"));
company.AddOrder(new Order(100, "Minsk - Gomel"));
company.AddOrder(new Order(300, "Minsk - Moscow"));
company.AddOrder(new Order(4080, "Minsk - Saint-Petersburg"));
company.AddOrder(new Order(5000, "Grodno - Brest"));
company.AddOrder(new Order(100, "Minsk - Mogilev"));
company.AddOrder(new Order(230, "Minsk - Warsaw"));
company.AddOrder(new Order(304, "Brest - Moscow"));
company.AddOrder(new Order(3531, "Minsk - Vilnius"));
company.AddOrder(new Order(11, "Grodno - Warsaw"));

company.RegisterClient(new Client("Sergey", "1234567890"));
company.RegisterClient(new Client("Vlad", "0987654321"));
company.RegisterClient(new Client("Pavel", "1111111111"));
company.RegisterClient(new Client("Vadzim", "2222222222"));
company.RegisterClient(new Client("Sasha", "3333333333"));
company.RegisterClient(new Client("Tima", "4444444444"));

company.PlaceOrderToClient(new Order(100, "Moscow - Saint-Petersburg"), "1234567890");
company.PlaceOrderToClient(new Order(200, "Minsk - Gomel"), "2222222222");
company.PlaceOrderToClient(new Order(11, "Grodno - Warsaw"), "4444444444");
company.PlaceOrderToClient(new Order(304, "Brest - Moscow"), "1234567890");
company.PlaceOrderToClient(new Order(3531, "Minsk - Vilnius"), "1234567890");
company.PlaceOrderToClient(new Order(4080, "Minsk - Saint-Petersburg"), "2222222222");
company.PlaceOrderToClient(new Order(100, "Minsk - Mogilev"), "4444444444");
company.PlaceOrderToClient(new Order(200, "Minsk - Gomel"), "0987654321");
company.PlaceOrderToClient(new Order(304, "Brest - Moscow"), "1234567890");
company.PlaceOrderToClient(new Order(300, "Minsk - Moscow"), "1234567890");

List<Order> orders = company.GetOrdersListSortedByCost();
Console.WriteLine("Orders sorted by cost:");
Console.WriteLine(string.Join(Environment.NewLine, orders));
Console.WriteLine(company.GetNameOfTheClientWithMaxOrdersCost());
Console.WriteLine(company.NumberOfClientsWhoPaidMoreThan(100));

Dictionary<string, int> d = company.AmoutsPaidForEachOrderForClient("1234567890");
foreach (var item in d)
{
    Console.WriteLine($"Order: {item.Key}, amount paid: {item.Value}");
}

journal.PrintRegisteredEvents();