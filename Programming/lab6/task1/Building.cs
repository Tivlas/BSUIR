using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task1
{
    abstract internal class Building
    {
        protected int floors;

        protected int numberOfEntrances;

        protected bool isTheHeatingOn;

        protected Building() { Console.WriteLine("Base building constructor was called\n"); }
        protected int Floors
        {
            get
            {
                return floors;
            }
            set
            {
                if (value < 1)
                {
                    floors = 1;
                }
                else
                {
                    floors = value;
                }
            }

        }
        public string? Address { get; set; }
        protected int NumberOfEntrances
        {
            get
            {
                return numberOfEntrances;
            }
            set
            {
                if (value < 1)
                {

                    numberOfEntrances = 1;
                }
                else
                {
                    numberOfEntrances = value;
                }
            }
        }

        public bool IsTheHeatingOn
        {
            get
            {
                return isTheHeatingOn;
            }
        }
        public void TurnOnTheHeating()
        {
            isTheHeatingOn = true;
            Console.WriteLine($"The heating is on in {GetType().Name}\n");
        }
        protected abstract void Build();

        public virtual void Repair()
        {
            Console.WriteLine($"Repairing the {GetType().Name}\n");
        }
    }
}
