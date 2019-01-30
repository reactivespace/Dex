#include <boost/test/unit_test.hpp>

#include <graphene/app/application.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/is_authorized_asset.hpp>

#include <graphene/chain/account_object.hpp>
#include <graphene/chain/asset_object.hpp>
#include <graphene/chain/tree.hpp>

#include <fc/crypto/digest.hpp>

#include "../common/database_fixture.hpp"
#include "../common/test_utils.hpp"

#include <iostream>
#include <string>

using namespace graphene::chain;
using namespace graphene::chain::test;

BOOST_FIXTURE_TEST_SUITE( maturity_tests, database_fixture )

BOOST_AUTO_TEST_CASE( mat_test )
{
   try {
      std::vector<account_test_in> test_accounts = {
            account_test_in("nathan", "committee-account", leaf_info()),
            account_test_in("dan", "committee-account", leaf_info())
      };

      CREATE_ACCOUNTS(test_accounts);
      generate_block();
      // db.get_dynamic_global_properties().next_maintenance_time
      db.modify(db.get_dynamic_global_properties(), [&](dynamic_global_property_object& dgpo) {
         dgpo.next_maintenance_time = db.head_block_time() + fc::hours(24);
      });
      const account_object& nathan = get_account("nathan");
      const account_object& dan = get_account("dan");

      generate_blocks(db.head_block_time() + fc::hours(6));
      transfer(account_id_type(), nathan.get_id(), asset(100000));
      transfer(account_id_type(), dan.get_id(), asset(100000));

      generate_blocks(db.head_block_time() + fc::hours(6));
      transfer(account_id_type(), nathan.get_id(), asset(100000));
      transfer(account_id_type(), dan.get_id(), asset(100000));

      generate_blocks(db.head_block_time() + fc::hours(6));
      transfer(account_id_type(), nathan.get_id(), asset(100000));
      transfer(account_id_type(), dan.get_id(), asset(100000));

      
      transfer(nathan.get_id(), account_id_type(), asset(150000));
      transfer(dan.get_id(), account_id_type(), asset(250000));

      BOOST_CHECK(get_mature_balance(nathan.get_id(), asset_id_type()) == 100000);
      BOOST_CHECK(get_mature_balance(dan.get_id(), asset_id_type()) == 37500);
   } catch(fc::exception& e) {
      edump((e.to_detail_string()));
      throw;
   }
}

BOOST_AUTO_TEST_SUITE_END()
