using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace HW1
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class AddItemPage : ContentPage
    {
        private const int max_limit = 100;
        private const int min_limit = 1;
        public struct SelectedItem
        {
            public string Name;
            public int Count;
        }
        public SelectedItem selectedItem 
        {
            get; private set;
        }
        public AddItemPage()
        {
            InitializeComponent();
            drinks.ItemsSource = new List<string>()
            {
                "Baikal",
                "Coca-cola"
            };
            drinks.SelectedIndex = 0;
        }


        private void IncreaseItem(string name, int count = 1)
        {
            selectedItem = new SelectedItem() { 
                Name = name, 
                Count = count 
            };
        }

        private void Button_Order(object sender, EventArgs e)
        {
            IncreaseItem(drinks.SelectedItem.ToString(), Int32.Parse(count.Text));
            Navigation.PopAsync();
        }

        private void Button_Minus(object sender, EventArgs e)
        {
            count.Text = ((Int32.Parse(count.Text)) - 1).ToString();
        }

        private void Button_Plus(object sender, EventArgs e)
        {
            count.Text = ((Int32.Parse(count.Text)) + 1).ToString();
        }

        private void count_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (UInt32.TryParse(e.NewTextValue, out var number))
            {
                if (number > max_limit)
                {
                    count.Text = max_limit.ToString();
                }
                else if (number < min_limit)
                {
                    count.Text = min_limit.ToString();
                }
                else 
                {
                    count.Text = number.ToString();
                }
                
            }
            else 
            {
                count.Text = e.OldTextValue;
            }
            
        }
    }
}