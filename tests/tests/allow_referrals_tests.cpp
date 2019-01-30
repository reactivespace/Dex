#include <boost/test/unit_test.hpp>

#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>

#include <graphene/chain/account_object.hpp>
#include <graphene/chain/asset_object.hpp>
#include <graphene/chain/committee_member_object.hpp>
#include <graphene/chain/proposal_object.hpp>

#include <fc/crypto/digest.hpp>

#include "../common/database_fixture.hpp"
#include "../common/test_utils.hpp"

using namespace graphene::chain;
using namespace graphene::chain::test;

BOOST_FIXTURE_TEST_SUITE( allow_referrals_tests, database_fixture )

BOOST_AUTO_TEST_CASE( allow_record_test )
{
   try {
      ACTOR(alice);

      transaction_evaluation_state evalState(&db);
      account_allow_referrals_operation ref_op;
      ref_op.target = alice_id;
      ref_op.action = account_allow_referrals_operation::allow;

      db.apply_operation(evalState, ref_op);

      auto& p = db.get_account_properties();
      auto itr = p.accounts_properties.find(alice_id);
      BOOST_CHECK(itr != p.accounts_properties.end());
      BOOST_CHECK(itr->second.can_be_referrer);
   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_CASE( disallow_no_exitant_record_test )
{
   try {
      ACTOR(alice);

      transaction_evaluation_state evalState(&db);
      account_allow_referrals_operation ref_op;
      ref_op.target = alice_id;
      ref_op.action = account_allow_referrals_operation::disallow;

      BOOST_REQUIRE_THROW(db.apply_operation(evalState, ref_op), fc::exception);
   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_CASE( disallow_exitant_record_test )
{
   try {
      ACTOR(alice);

      transaction_evaluation_state evalState(&db);
      account_allow_referrals_operation ref_op;
      ref_op.target = alice_id;
      ref_op.action = account_allow_referrals_operation::allow;
      db.apply_operation(evalState, ref_op);

      account_allow_referrals_operation ref_op_dis;
      ref_op_dis.target = alice_id;
      ref_op_dis.action = account_allow_referrals_operation::disallow;
      db.apply_operation(evalState, ref_op_dis);

      auto& p = db.get_account_properties();
      auto itr = p.accounts_properties.find(alice_id);
      BOOST_CHECK(itr != p.accounts_properties.end());
      BOOST_CHECK(!itr->second.can_be_referrer);
   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_CASE( allow_creation_before_fork )
{
   try {
      ACTORS((alice) (bob) (john));
      
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME - 30));

      transfer(committee_account, alice_id, asset(30000000, asset_id_type()));

      upgrade_to_lifetime_member(alice_id);

      BOOST_REQUIRE_NO_THROW(create_account("stub", alice, alice, 80, generate_private_key("stub").get_public_key()));

   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_CASE( allow_creation_after_fork_with_permission )
{
   try {
      ACTORS((alice) (bob) (john));
      
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));

      transaction_evaluation_state evalState(&db);
      account_allow_referrals_operation ref_op;
      ref_op.target = alice_id;
      ref_op.action = account_allow_referrals_operation::allow;

      db.apply_operation(evalState, ref_op);

      transfer(committee_account, alice_id, asset(30000000, asset_id_type()));

      upgrade_to_lifetime_member(alice_id);

      create_account("stub", get_account("alice"), get_account("alice"), 
                              80, generate_private_key("stub").get_public_key());

   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_CASE( disallow_creation_after_fork )
{
   try {
      ACTORS((alice) (bob) (john));
      
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));

      transfer(committee_account, alice_id, asset(30000000, asset_id_type()));

      upgrade_to_lifetime_member(alice_id);

      BOOST_REQUIRE_THROW(create_account("stub", alice, alice, 80, generate_private_key("stub").get_public_key()), fc::exception);

   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_CASE( allow_creation_after_fork_with_canceled_permission )
{
   try {
      ACTORS((alice) (bob) (john));

      generate_block();

      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));


      transfer(committee_account, alice_id, asset(30000000, asset_id_type()));

      upgrade_to_lifetime_member(alice_id);

      transaction_evaluation_state evalState(&db);
      account_allow_referrals_operation ref_op;
      ref_op.target = alice_id;
      ref_op.action = account_allow_referrals_operation::allow;


      db.apply_operation(evalState, ref_op);

      account_allow_referrals_operation ref_op_dis;
      ref_op_dis.target = alice_id;
      ref_op_dis.action = account_allow_referrals_operation::disallow;
      db.apply_operation(evalState, ref_op_dis);

      BOOST_REQUIRE_THROW(create_account("stub", get_account("alice"), get_account("alice"), 
                           80, generate_private_key("stub").get_public_key()), fc::exception);

   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_SUITE_END()
