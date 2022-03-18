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
        }

        private void ButtonBaikal_Clicked(object sender, EventArgs e)
        {
            IncreaseItem("Baikal");
        }

        private void ButtonColaCola_Clicked(object sender, EventArgs e)
        {
            IncreaseItem("Cola");
        }

        private void IncreaseItem(string name)
        {
            int count = 1;
            MainPage.items.Where(x => x.item_name == name).ToList().ForEach(x => 
            {
                count = ++x.item_count;
                MainPage.items.Remove(x);   
            });
            MainPage.items.Add(new Item(name, count, id++));
        }
    }
}