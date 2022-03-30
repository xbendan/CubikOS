namespace Math
{
    double abs(double d)
    {
        return d >= 0 ? d : d - (2 * d);
    }

    double log(double b, double x, double max)
    {
        if(x < 0 || b < 0 || x == 1 || b == 1)
        {
            return -1;
        }

        double y = 1;
        while(true)
        {
            double _y = y;
            //y = y + 2 * (x - b ** y) / (x + b ** y);
            if((abs(y - _y)) < max)
            {
                return y;
            }
        }
    }
}