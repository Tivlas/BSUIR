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

	protected override void OnModelCreating(ModelBuilder modelBuilder)
	{
		modelBuilder.Entity<Clothes>().HasKey(cl => cl.Id);
		modelBuilder.Entity<Clothes>().Property(cl => cl.Id).ValueGeneratedOnAdd();
		modelBuilder.Entity<Clothes>().HasOne(cl => cl.Category).WithMany(cc => cc.Clothes).HasForeignKey(cl => cl.CategoryId);

		modelBuilder.Entity<ClothesCategory>().HasKey(cc => cc.Id);
		modelBuilder.Entity<ClothesCategory>().Property(cc => cc.Id).ValueGeneratedOnAdd();
	}
}
