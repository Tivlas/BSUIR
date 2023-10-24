using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using Domain.Entities;

namespace Domain.Models;
public class CartItem
{
    public Clothes Clothes { get; set; }
    public int Quantity { get; set; }
}
