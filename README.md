# CoreLib
CoreLib is built as a general C++ application framework, which lies as the basis of most of [Zefiros Software](www.zefiros.eu) programs.

# Status
Type        | Status
----------- | -------
Linux & OSX | [![Build Status](https://travis-ci.org/Zefiros-Software/CoreLib.svg?branch=master)](https://travis-ci.org/Zefiros-Software/CoreLib)
Windows     | [![Build status](https://ci.appveyor.com/api/projects/status/c33agdcblb6wl7ro?svg=true)](https://ci.appveyor.com/project/Zefiros-Software/CoreLib)
Coverage	| [![codecov](https://codecov.io/gh/Zefiros-Software/CoreLib/branch/master/graph/badge.svg)](https://codecov.io/gh/Zefiros-Software/CoreLib)

# [ZPM](http://zpm.zefiros.eu) Installation
In `.package.json`
```json
"requires": [
		{
			"name": "Zefiros-Software/CoreLib",
			"version": "@head"
		}
]
```

In `premake5.lua`
```lua
zpm.uses "Zefiros-Software/CoreLib"
```

## Bugs
When a bug is found, please insert it in the issue tracker, so we can resolve it as quickly as we can.

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request

## License
This project is licensed under the MIT license by [Zefiros Software](https://zefiros.eu).

```
Copyright (c) 2017 Zefiros Software.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```