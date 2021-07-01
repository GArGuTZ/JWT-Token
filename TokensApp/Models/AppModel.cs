using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using Microsoft.EntityFrameworkCore;

namespace TokenApp.Models
{
    public class User
    {
        public int Id { get; set; }
        public string Login { get; set; }
        public string Password { get; set; }
        public List<Message> Messages { get; set; }
    }
    public class Message
    {
        public int Id { get; set; }
        public string Content { get; set; }
        public int UserId { get; set; } // Foreign Key
        public User User { get; set; }
    }
    public class ApplicationContext : DbContext
    {
        public DbSet<User> Users { get; set; }
        public DbSet<Message> Messages { get; set; }
        public ApplicationContext()
        {
            //Database.EnsureDeleted();
            Database.EnsureCreated();
        }
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer(@"Server=(localdb)\mssqllocaldb;Database=relationsdb;Trusted_Connection=True;");
        }
        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Message>()
                .HasOne(p => p.User)
                .WithMany(t => t.Messages)
                .HasForeignKey(p => p.UserId);
            modelBuilder.Entity<User>().HasData(
            new User[]
            {
                new User { Id=1, Login="First", Password="11111"},
                new User { Id=2, Login="Second", Password="22222"},
                new User { Id=3, Login="Admin", Password="333"}
            });
        }
    }
}