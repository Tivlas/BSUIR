using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Domain.Entities;

namespace Domain.Models;
public class Cart
{
	public Dictionary<int, CartItem> CartItems { get; set; } = new();

	public virtual void Add(Clothes clothes)
	{
		if (!CartItems.ContainsKey(clothes.Id))
		{
			CartItems.Add(clothes.Id, new CartItem() { Clothes = clothes, Quantity = 1 });
		}
		else
		{
			++CartItems[clothes.Id].Quantity;
		}
	}

	public virtual void Remove(int id)
	{
		if (--CartItems[id].Quantity <= 0)
		{
			CartItems.Remove(id);
		}
	}

	public virtual void Clear()
	{
		CartItems.Clear();
	}

	public int Quantity => CartItems.Sum(item => item.Value.Quantity);

	public decimal TotalPrice => CartItems.Sum(item => item.Value.Clothes.Price * item.Value.Quantity);

}
