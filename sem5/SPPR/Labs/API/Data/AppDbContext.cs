using Domain.Entities;
using Microsoft.EntityFrameworkCore;

namespace API.Data;

public class AppDbContext : DbContext
{
    public DbSet<Clothes> Clothes { get; set; }	
    public DbSet<ClothesCategory> ClothesCategory { get; set; }

	public AppDbContext(DbContextOptions<AppDbContext> options)
			: base(options)
	{
		Database.EnsureCreated();
	}
}
