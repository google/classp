#ifndef CLASSP_H_
#define CLASSP_H_

#include <assert.h>
#include <functional>
#include <iostream>
#include <string>
#include <map>
#include <vector>

namespace classp {

// Common Base class for all AST nodes. Each classp file also generates its own
// base class that inherits from ClasspNode.
class ClasspNode {
 public:

  // Returns the name of this class.
  virtual string className() { return "ClasspNode"; }

  virtual ~ClasspNode() {}

  // Writes out a human-readable tree structure of the AST.
  void print(std::ostream& out) {
    out << "(" << className();
    printMembers(out);
    out << ")";
  }

  // Prints the members of this node for print().
  virtual void printMembers(std::ostream& out) {}

  // Writes the AST in the declared syntax.
  virtual void format(std::ostream& out, int precedence) = 0;

  // Writes the AST in the declared syntax starting with no precedence.
  virtual void format(std::ostream& out) {
    format(out, 0);
  }
};

// Helper functions for ClasspNode::print() and ClasspNode::format() that can handle
// null trees and leaf values that are not ClasspNodes.
template<class T> void classpPrint(std::ostream& out, T val) {
  out << val;
}
template<class T> void classpPrint(std::ostream& out, std::vector<T> arr) {
  const char* separator = "";
  out << "[";
  for (const auto& val : arr) {
    out << separator;
    classpPrint(out, val);
    separator = ", ";
  }
  out << "]";
}
template<class T> void classpPrint(std::ostream& out, T* val) {
  if (val) val->print(out);
  else out << "nullptr";
}

template<class T> void classpFormat(std::ostream& out, int precedence, T val) {
  out << val;
}
template<class T> void classpFormat(std::ostream& out, int precedence, ClasspNode* val) {
  if (val) val->format(out, precedence);
  else out << "***MISSING***";  // this is an error case
}

// RestrictedUnion is a restricted variant class that must be read exactly
// once and can't be copied, only moved. That is, the object must
// not be destroyed until it has either been copied (with the copy constructor)
// or the value has been extracted with Take().
class RestrictedUnion {
 public:
  template <class T> RestrictedUnion(const T& value) {
    data = (void*)new T(value);
  }

  // The copy constructor moves the value
  RestrictedUnion(const RestrictedUnion& old) {
    data = old.data;
    old.data = nullptr;
  }

  ~RestrictedUnion() {
    assert(!data);
  }

  // Return the attribute value.
  template<class T> const T& Ref() {
    assert(data);
    return *(T*)data;
  }

  // Get the attribute value while destroying the copy in the variant.
  template<class T> void Take(T& value) {
    assert(data);
    value = *(T*)data;
    delete (T*)data;
    data = nullptr;
  }

  RestrictedUnion& operator=(RestrictedUnion&) = delete;

  mutable void* data;
};

// AttributeMap implements a mapping from attribute names to values. It is
// used to simulate keyword parameters in the constructors for the ClasspNode
// classes and also to build up the set of keyword parameters during parsing.
class AttributeMap {
 public:
  // Add all of the elements of map2 into this, deleting map2
  void Merge(AttributeMap& map2) {
    for (auto iter = map2.values.begin(); iter != map2.values.end(); iter++) {
      values.emplace(iter->first, iter->second);
    }
    // Because the RestrictedUnion objects were copied out of map2 using the
    // destructive copy operator, they are now all blank.
    map2.values.clear();
  }

  // Push value onto the array attribute, creating a new array if needed.
  template<class T> void Push(const std::string& attribute, T value) {
    auto iterator = values.find(attribute);
    if (values.count(attribute) == 0) {
      iterator = values.emplace(attribute, std::vector<T>()).first;
    }
    iterator->second.Ref< std::vector<T> >().push_back(value);
  }

  // Set the attribute to value. It is an error if attribute is already set.
  template<class T> void Add(const std::string& attribute, T value) {
    values.emplace(attribute, value);
  }

  template<class T> bool Take(const std::string& attribute, T& value) {
    auto iter = values.find(attribute);
    if (iter == values.end()) return false;
    iter->second.Take(value);
    return true;
  }

  std::map<const std::string, RestrictedUnion> values;
};

}  // namespace classp

#endif  // CLASSP_H_
