
#pragma warning(disable: 4251)

/* -*- C++ -*- */

#ifndef _CBASE_ACK_H_
#define _CBASE_ACK_H_
#include <list>
#include <map>

using namespace std;

#ifdef _DLL_TRANSLATE                  //                  //
#define LANG_EXPORT __declspec(dllexport)                   //                  //
#else                                   //                  //
#define LANG_EXPORT                     //                  //
#endif  

template <typename T>
class LANG_EXPORT CBase_ACK
{
public:
    typedef typename list<T*>::iterator iterator;
    typedef typename list<T*>::size_type size_type;
    CBase_ACK() {}
    virtual ~CBase_ACK() {Empty();}
    void Empty()
        {
            for (iterator iter = m_list.begin(); iter != m_list.end(); ++iter)
            {
				_DisposeIterator(iter);
            }

            m_list.clear();
        }
    typename list<T*>::iterator begin()
        {
            return m_list.begin();;
        }
    typename list<T*>::iterator end()
        {
            return m_list.end();
        }
    typename list<T*>::size_type size() const
        {
            return m_list.size();
        }

    typename list<T*>::iterator erase(typename list<T*>::iterator _where)
        {
            _DisposeIterator(_where);
            return m_list.erase(_where);
        }

	virtual void _DisposeIterator(typename list<T*>::iterator _where)
	{
		if ((*_where) != NULL){
			delete *_where;
			*_where = NULL;
		}
	}


    list<T*> m_list;
};

#endif /* _CBASE_ACK_H_ */ 
