﻿using System.Text.Json.Serialization;
using Domain.Entities;
using Domain.Models;
using WEB_153505_Vlasenko.Extensions;

namespace WEB_153505_Vlasenko.Services.CartService;

public class SessionCart : Cart
{
	public static Cart GetCart(IServiceProvider services)
	{
		ISession? session = services.GetRequiredService<IHttpContextAccessor>().HttpContext?.Session;
		SessionCart cart = session?.Get<SessionCart>("Cart") ?? new SessionCart();
		cart.Session = session;
		return cart;
	}

	[JsonIgnore]
	public ISession? Session { get; set; }
	public override void Add(Clothes product)
	{
		base.Add(product);
		Session?.Set("Cart", this);
	}

	public override void Remove(int id)
	{
		base.Remove(id);
		Session?.Set("Cart", this);
	}

	public override void Clear()
	{
		base.Clear();
		Session?.Remove("Cart");
	}
}
