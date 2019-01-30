For Ubuntu users, see this link first:

   https://github.com/edincoin/EDINARCOIN/wiki/build-ubuntu

Windows: 

   https://github.com/edincoin/EDINARCOIN/wiki/BUILD_WIN32

OS X: 

   https://github.com/edincoin/EDINARCOIN/wiki/Building-on-OS-X

and then proceed with:

    git clone https://github.com/edincoin/EDINARCOIN.git
    cd EDINARCOIN
    git submodule update --init --recursive
    cmake .
    make
    programs/witness_node/witness_node --data-dir=data/witness --seed-node=138.201.192.132:5908 --rpc-endpoint=0.0.0.0:8899 --genesis-json=genesis.json

Then, in a separate terminal window, start the command-line wallet `cli_wallet`:

    programs/cli_wallet/cli_wallet --server-rpc-endpoint=ws://127.0.0.1:8899

To set password:

    >>> set_password password
    >>> unlock password

To import your key:
    
    >>> import_key your_account_name your_private_key true
To import your initial balance:

    >>> import_balance your_account_name [your_private_key] true

If you send private keys over this connection, `rpc-endpoint` should be bound to localhost for security.

A list of CLI wallet commands is available here:
    
   https://github.com/edincoin/EDINARCOIN/blob/master/libraries/wallet/include/graphene/wallet/wallet.hpp

Information about delayed_node is availiable here:

   https://github.com/edincoin/EDINARCOIN/wiki/Delayed%20node