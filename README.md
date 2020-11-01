# Verified Blockchain

![Language](https://img.shields.io/badge/language-C++14-blue)
[![LICENSE](https://img.shields.io/github/license/verified-network/verified-blockchain)](LICENSE)
![Status](https://img.shields.io/badge/status-unstable-red)
[![Website](https://img.shields.io/website?down_color=lightgrey&down_message=offline&up_color=blue&up_message=online&url=https://www.verified.network/)](https://www.verified.network/)
<!-- [![GitHub Issues](https://img.shields.io/github/issues/verified-network/verified-blockchain)](https://github.com/verified-network/verified-blockchain/issues) -->
<!-- ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/verified-network/verified-blockchain) -->
<!-- ![Sonar Quality Gate](https://img.shields.io/sonar/:metric/:component?server=https%3A%2F%2Fsonarcloud.io&sonarVersion=4.2) -->

> This is an implementation of the Verified blockchain consensus protocol on a DHT based network.

## Contact

- Report bugs, issues or feature requests using [GitHub issues](https://github.com/verified-network/verified-blockchain/issues/new).

## Install

### Building from the source

#### Get the source code

Git and Github are used to maintain the source code. Clone the repository by:

```shell
git clone https://github.com/verified-network/verified-blockchain.git

cd verified-blockchain
```

#### Build

On **Windows** we support Visual Studio 2017, and 2019. 

* **Visual Studio 2019**: open verified-blockchain.sln and build to x86 target.


## License

[![LICENSE](https://img.shields.io/github/license/verified-network/verified-blockchain)](LICENSE)

All contributions are made under the [MIT License](https://opensource.org/licenses/MIT). See [LICENSE](LICENSE).

## Things to be Added

- [x] Integration with openDHT
- [x] Integration with Verified storage (a patricia merkle trie implemented in C++)
- [x] Implement Verified blockchain consensus protocol
- [ ] Test with Verified storage
- [ ] Optimize openDHT for performance
- [ ] Integrate with crun based Verified run time 
- [ ] Perform Static Code Analysis
- [ ] Check memory footprint of the entire library


**NOTE:** Currently this library is under development and unstable, NOT FOR PRODUCTION.
