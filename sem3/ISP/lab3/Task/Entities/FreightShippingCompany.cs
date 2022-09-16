using System;
using System.Collections.Generic;
using CustomExceptions;
using CompanyEvents;
using System.Linq;

namespace Task
{
    public class FreightShippingCompany
    {
        public event EventHandler<CompanyEventArgs>? OrdersOrClientsListChanged;
        public event EventHandler<CompanyEventArgs>? ClientPlacedAnOrder;

        private Dictionary<string, Order> _availableOrders = new();
        private List<Client> _clients = new();

        public void AddOrder(Order order)
        {
            _availableOrders.Add(order.Route, order);
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEventArgs($"Order \"{order}\" was added to the list of available orders"));
        }

        public void RemoveOrder(Order order)
        {
            bool wasRemoved = _availableOrders.Remove(order.Route);
            if (!wasRemoved)
            {
                throw new OrderNotFoundException("Trying to remove an order that doesn't exist");
            }
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEventArgs($"Order \"{order}\" was removed from the list of available orders"));
        }

        public void RegisterClient(Client client)
        {
            _clients.Add(client);
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEventArgs($"Client \"{client}\" was registered"));
        }

        public void RemoveClient(Client client)
        {
            bool wasRemoved = _clients.Remove(client);
            if (!wasRemoved)
            {
                throw new ClientNotFoundException("Trying to remove a client that is not registered");
            }
            OrdersOrClientsListChanged?.Invoke(this, new CompanyEventArgs($"Client \"{client}\" was removed from the list of registered clients"));
        }


        public int CalculateTotalOrdersCostForClient(string clientID)
        {
            int totalCost = 0;
            var clientToCalcCostFor = _clients.Find((client) => client.ID == clientID);
            if (clientToCalcCostFor == null)
            {
                {
                    throw new ClientNotFoundException("Trying to calculate total cost for a client that is not registered");
                }
            }
            totalCost += clientToCalcCostFor.CalculateTotalOrdersCost();
            return totalCost;
        }

        public void PlaceOrderToClient(Order order, string clientID)
        {
            var clientToPlaceOrderTo = _clients.Find((client) => client.ID == clientID);
            if (clientToPlaceOrderTo == null)
            {
                throw new ClientNotFoundException("Trying to place an order to a client that is not registered");
            }
            if (!_availableOrders.ContainsKey(order.Route))
            {
                throw new OrderNotFoundException("Trying to place an order that doesn't exist");
            }
            clientToPlaceOrderTo.PlaceOrder(order);
            ClientPlacedAnOrder?.Invoke(this, new CompanyEventArgs($"Order \"{order}\" was placed to client \"{clientToPlaceOrderTo}\""));
        }

        public List<Order> GetOrdersListSortedByCost()
        {
            return (from keyValuePair in _availableOrders orderby keyValuePair.Value.Cost select keyValuePair.Value).ToList();
        }

        public int CalculateTotalOrdersCostForAllClients()
        {
            return (from client in _clients select client.CalculateTotalOrdersCost()).Sum();
        }

        public string GetNameOfTheClientWithMaxOrdersCost()
        {
            return (from client in _clients orderby client.CalculateTotalOrdersCost() descending select client).First().Name;
        }

        public int NumberOfClientsWhoPaidMoreThan(int cost)
        {
            return _clients.Aggregate(0, (curNumberOfClients, client) => curNumberOfClients + (client.CalculateTotalOrdersCost() > cost ? 1 : 0));
        }

        public Dictionary<string, int> AmoutsPaidForEachOrderForClient(string clientID)
        {
            var client = _clients.Find((client) => client.ID == clientID);
            if (client == null)
            {
                throw new ClientNotFoundException("Trying to get orders for a client that is not registered");
            }
            return client.GetListOfAmountsPaidForEachOrder();
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