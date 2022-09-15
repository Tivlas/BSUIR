using System;

namespace Task
{

    public class FreightShippingCompany
    {
        public event EventHandler<CompanyEvents.CompanyEventArgs>? OrdersOrClientsListChanged;
        public event EventHandler<CompanyEvents.CompanyEventArgs>? ClientPlacedAnOrder;

        private MyCustomCollection<Order> _availableOrdersList = new MyCustomCollection<Order>();
        private MyCustomCollection<Client> _clients = new MyCustomCollection<Client>();

        public void AddOrder(Order order)
        {
            _availableOrdersList.Add(order);
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEvents.CompanyEventArgs($"Order \"{order}\" was added to the list of available orders"));
        }

        public void RemoveOrder(Order order)
        {
            _availableOrdersList.Remove(order);
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEvents.CompanyEventArgs($"Order \"{order}\" was removed from the list of available orders"));
        }

        public void RegisterClient(Client client)
        {
            _clients.Add(client);
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEvents.CompanyEventArgs($"Client \"{client}\" was registered"));
        }

        public void RemoveClient(Client client)
        {
            _clients.Remove(client);
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEvents.CompanyEventArgs($"Client \"{client}\" was removed from the list of registered clients list"));
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
                throw new CustomExceptions.OrderNotFoundException($"Order {order} is not available.");
            }
            client.PlaceOrder(order);
            ClientPlacedAnOrder?.Invoke(this, new CompanyEvents.CompanyEventArgs($"Client \"{client}\" placed an order \"{order}\""));
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

namespace CompanyEvents
{
    public class CompanyEventArgs : EventArgs
    {
        public readonly string Message;
        public CompanyEventArgs(string message)
        {
            Message = message;
        }
    }
}