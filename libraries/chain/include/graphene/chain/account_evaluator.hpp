/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/account_object.hpp>

namespace graphene { namespace chain {

class account_create_evaluator : public evaluator<account_create_evaluator>
{
public:
   typedef account_create_operation operation_type;

   void_result do_evaluate( const account_create_operation& o );
   object_id_type do_apply( const account_create_operation& o ) ;
};

class account_update_evaluator : public evaluator<account_update_evaluator>
{
public:
   typedef account_update_operation operation_type;

   void_result do_evaluate( const account_update_operation& o );
   void_result do_apply( const account_update_operation& o );

   const account_object* acnt;
};

class account_upgrade_evaluator : public evaluator<account_upgrade_evaluator>
{
public:
   typedef account_upgrade_operation operation_type;

   void_result do_evaluate(const operation_type& o);
   void_result do_apply(const operation_type& o);

   const account_object* account;
};

class account_whitelist_evaluator : public evaluator<account_whitelist_evaluator>
{
public:
   typedef account_whitelist_operation operation_type;

   void_result do_evaluate( const account_whitelist_operation& o);
   void_result do_apply( const account_whitelist_operation& o);

   const account_object* listed_account;
};

class account_restrict_evaluator : public evaluator<account_restrict_evaluator>
{
public:
   typedef account_restrict_operation operation_type;

   void_result do_evaluate( const account_restrict_operation& o);
   object_id_type do_apply( const account_restrict_operation& o);

   
   const restricted_account_object* restricted_account = nullptr;
};

class account_allow_referrals_evaluator : public evaluator<account_allow_referrals_evaluator>
{
public:
   typedef account_allow_referrals_operation operation_type;

   void_result do_evaluate( const account_allow_referrals_operation& o);
   object_id_type do_apply( const account_allow_referrals_operation& o);

};

class set_online_time_evaluator : public evaluator<set_online_time_evaluator>
{
public:
   typedef set_online_time_operation operation_type;

   void_result do_evaluate( const set_online_time_operation& o);
   void_result do_apply( const set_online_time_operation& o);

};

class set_verification_is_required_evaluator : public evaluator<set_verification_is_required_evaluator>
{
public:
   typedef set_verification_is_required_operation operation_type;

   void_result do_evaluate( const set_verification_is_required_operation& o);
   void_result do_apply( const set_verification_is_required_operation& o);

};

} } // graphene::chain
