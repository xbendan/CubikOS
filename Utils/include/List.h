namespace Utils {
    template <typename T> class List {
        virtual void Add(const T& obj) = 0;
        virtual void Insert(int index, const T& obj) = 0;
        virtual T& Set(int index, const T& obj) = 0;
        virtual void Get(int index, const T& obj) = 0;
        virtual unsigned int Size() = 0;
        virtual void Remove(int index) = 0;
        
    }
}