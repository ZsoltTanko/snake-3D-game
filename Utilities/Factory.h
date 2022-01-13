#ifndef FACTORY_H
#define FACTORY_H



// TODO:
// - Error handling


// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include <map>


// ------------------------------------------------------------------------------------
// --------------------Templatized class declarations & definitions--------------------
// ------------------------------------------------------------------------------------

template <class AbstractProduct,
		  typename IDType,
		  typename ProductCreator = AbstractProduct *(*)()
		 >
class Factory
{
public:
	// Register a creation function with a type ID
	bool Register(const IDType &ID, const ProductCreator Creator)
	{
		return Associations.insert(make_pair(ID, Creator)).second;
	}
	// Unregister a type
	bool Unregister(const IDType &ID)
	{
		return Associations.erase(ID) == 1;
	}
	// Create a product based on its type ID
	AbstractProduct *CreateProduct(const IDType &ID)
	{
		// Get iterator
		typename AssociationMap::const_iterator it = Associations.find(ID);

		// Find product based on ID
		if (it != Associations.end())
			return (it->second)();

		// TODO- Fix error handling
		return NULL;
	}

private:
	typedef std::map<IDType, ProductCreator> AssociationMap;
	AssociationMap Associations;
};

// Helper class used to register concrete product IDs with a factory
template <class AbstractProduct, class ConcreteProduct, typename IDType>
class FactoryRegistrar
{
public:
    // Type for creator function
    typedef AbstractProduct *(*CreatorFunction)();

    // Registration function
    FactoryRegistrar( const IDType ID, const CreatorFunction Creator = NewCreate)
    {
        Singleton<Factory<AbstractProduct, IDType>>::Instance().Register( ID, Creator );
    }

private:
    // Default creation function
    static AbstractProduct *NewCreate()
    {
        return new ConcreteProduct;
    }
};



#endif