// -*- C++ -*-

//=============================================================================
/**
 *  @file    be_template_module.h
 *
 *  Extension of class AST_Template_Module that provides additional means for C++
 *  mapping of a module.
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BE_TEMPLATE_MODULE_H
#define TAO_BE_TEMPLATE_MODULE_H

#include "be_scope.h"
#include "be_decl.h"

#include "ast_template_module.h"

/**
 * The back end extension of the AST_Template_Module class
 */
class be_template_module : public virtual AST_Template_Module,
                           public virtual be_scope,
                           public virtual be_decl
{
public:
  be_template_module (
    UTL_ScopedName *n,
    FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~be_template_module ();

  virtual void destroy ();

  virtual int accept (be_visitor *visitor);
};

#endif /* TAO_BE_TEMPLATE_MODULE_H */

