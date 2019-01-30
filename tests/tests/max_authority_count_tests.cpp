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

BOOST_FIXTURE_TEST_SUITE( keys_and_accounts_usage, database_fixture )

BOOST_AUTO_TEST_CASE( disallow_more_than_3 )
{
   try {
      ACTORS((alice) (bob) (john));
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));


      transfer(committee_account, alice_id, asset(30000000, asset_id_type()));

      upgrade_to_lifetime_member(alice_id);

      transaction_evaluation_state evalState(&db);
      account_allow_referrals_operation ref_op;
      ref_op.target = alice_id;
      ref_op.action = account_allow_referrals_operation::allow;

      db.apply_operation(evalState, ref_op);

      create_account("stub1", get_account("alice"), get_account("alice"), 
                           80, generate_private_key("stub").get_public_key());
      create_account("stub2", get_account("alice"), get_account("alice"), 
                           80, generate_private_key("stub").get_public_key());
      create_account("stub3", get_account("alice"), get_account("alice"), 
                           80, generate_private_key("stub").get_public_key());
      BOOST_REQUIRE_THROW(create_account("stub", get_account("alice"), get_account("alice"), 
                           80, generate_private_key("stub").get_public_key()), fc::exception);

   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_CASE( disallow_more_than_3_keys_on_update )
{
   try {
      ACTORS((alice) (bob) (john));

      transfer(committee_account, alice_id, asset(30000000, asset_id_type()));

      upgrade_to_lifetime_member(alice_id);

      auto acc_alice = get_account("alice");
      auto acc_bob = get_account("bob");

      create_account("stub", acc_alice, acc_alice, 
                           80, generate_private_key("abcaa").get_public_key());
      create_account("stub1", acc_alice, acc_alice, 
                           80, generate_private_key("abcaa1").get_public_key());
      create_account("stub2", acc_alice, acc_alice, 
                           80, generate_private_key("abcaa2").get_public_key());
                           
      auto acc1 = get_account("stub");
      auto acc2 = get_account("stub1");
      auto acc3 = get_account("stub2");
      

      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));
      generate_blocks(fc::time_point_sec(HARDFORK_617_TIME));

      set_expiration(db, this->trx);
      transaction_evaluation_state evalState(&db);
      account_update_operation op;
      op.account = acc2.id;
      op.active = authority(1, acc1.get_id(), 1);
      op.new_options = acc2.options;
      trx.operations.push_back(op);
      PUSH_TX( db, trx, ~0 );
      trx.operations.clear();

      op.account = acc3.id;
      op.active = authority(1, acc1.get_id(), 1);
      op.new_options = acc3.options;
      trx.operations.push_back(op);
      PUSH_TX( db, trx, ~0 );
      trx.operations.clear();

      op.account = acc_bob.id;
      op.active = authority(1, acc1.get_id(), 1);
      op.new_options = acc_bob.options;
      trx.operations.push_back(op);
      PUSH_TX( db, trx, ~0 );
      trx.operations.clear();

      op.account = acc_alice.id;
      op.active = authority(1, acc1.get_id(), 1);
      op.new_options = acc_alice.options;
      BOOST_REQUIRE_THROW(db.apply_operation(evalState, op), fc::exception);

   } catch (fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_SUITE_END()
