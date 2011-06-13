#ifndef RBL_EN_HOME_TYPE_EVENT_H
#define RBL_EN_HOME_TYPE_EVENT_H

namespace rubble { namespace event_model {
  class rbl_event
  {
#if 0
    // 
    template<typename T>
    void set_reference(T & reference)
    {
      ptr = (void *) reference;
    }
    template<typename T>
    T & get_reference()
    {
      return ( (T) *ptr);
    }
  private:
    void * ptr;
#endif 
  };

  template<>
  struct rbl_type_type_traits<rbl_event>
  {
    struct rbl_event_dsl_strings
    {
      int count()
      {
        return 1;
      }
      const char ** operator()()
      {
        static const char * r[] = {"event"};
        return  r;
      }  
    };
    typedef rbl_event value_type;
    typedef rbl_event_dsl_strings dsl_strings;
  };
} }
#endif 


