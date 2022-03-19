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
        private static int id = 0;
        public AddItemPage()
        {
            InitializeComponent();
            drinks.ItemsSource = new List<string>()
            {
                "Baikal",
                "Coca-cola"
            };
        }


        private void IncreaseItem(string name, int count = 1)
        {
            MainPage.items.Where(x => x.item_name == name).ToList().ForEach(x =>
            {
                count += x.item_count;
                MainPage.items.Remove(x);
            });
            MainPage.items.Add(new Item(name, count, id++));
        }

        private void Button_Order(object sender, EventArgs e)
        {
            IncreaseItem(drinks.SelectedItem.ToString(), Int32.Parse(count.Text));
        }

        private void Button_Minus(object sender, EventArgs e)
        {
            count.Text = ((Int32.Parse(count.Text)) - 1).ToString();
        }

        private void Button_Plus(object sender, EventArgs e)
        {
            count.Text = ((Int32.Parse(count.Text)) + 1).ToString();
        }
    }
}