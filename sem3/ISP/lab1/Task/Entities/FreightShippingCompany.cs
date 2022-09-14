using System;

namespace Task
{

    public class FreightShippingCompany
    {
        private MyCustomCollection<Order> _availableOrdersList = new MyCustomCollection<Order>();
        private MyCustomCollection<Client> _clients = new MyCustomCollection<Client>();

        public void AddOrder(Order order)
        {
            _availableOrdersList.Add(order);
        }

        public void RemoveOrder(Order order)
        {
            _availableOrdersList.Remove(order);
        }

        public void RegisterClient(Client client)
        {
            _clients.Add(client);
        }

        public void RemoveClient(Client client)
        {
            _clients.Remove(client);
        }

        private Client? FindClient(string id)
        {
            _clients.Reset();
            while (_clients.MoveNext())
            {
                if (_clients.Current()!.ID == id)
                {
                    return _clients.Current();
                }
            }
            return null;
        }

        public int CalculateTotalOrdersCostForClient(string clientID)
        {
            int totalCost = 0;
            var client = FindClient(clientID);

            if (client == null)
            {
                throw new CustomExceptions.ClientNotFoundException("Client with ID " + clientID + " not found");
            }
            totalCost = client.CalculateTotalOrdersCost();
            return totalCost;
        }

        private bool IsOrderAvailable(Order order)
        {
            _availableOrdersList.Reset();
            while (_availableOrdersList.MoveNext())
            {
                if (order.Equals(_availableOrdersList.Current()))
                {
                    return true;
                }
            }
            return false;
        }

        public void PlaceOrderToClient(Order order, string clientID)
        {
            var client = FindClient(clientID);
            if (client == null)
            {
                throw new CustomExceptions.ClientNotFoundException($"Client with ID {clientID} not found.");
            }
            if (!IsOrderAvailable(order))
            {
                throw new CustomExceptions.OrderNotFoundException($"Order {order.Route} is not available.");
            }
            client.PlaceOrder(order);
        }

        public int CalculateTotalOrdersCostForAllClients()
        {
            int totalCost = 0;
            _clients.Reset();
            while (_clients.MoveNext())
            {
                totalCost += _clients.Current()!.CalculateTotalOrdersCost();
            }
            return totalCost;
        }
    }
}

namespace CustomExceptions
{
    public class OrderNotFoundException : Exception
    {
        public OrderNotFoundException(string message) : base(message)
        {
        }
    }

    public class ClientNotFoundException : Exception
    {
        public ClientNotFoundException(string message) : base(message)
        {
        }
    }
}