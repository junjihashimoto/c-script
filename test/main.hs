{-#LANGUAGE TemplateHaskell#-}
{-#LANGUAGE QuasiQuotes#-}

import Test.Hspec
import Text.Heredoc
import System.Process

(@==) (prog,args,input) exp = 
   readProcess prog args input `shouldReturn` exp

main :: IO ()
main = hspec $ do
  describe "basic-test" $ do
    it "help" $ 
      ("./c-script",["./sample/test/test.cpp"],[]) @== 
        [str|hello world
            |]
    it "hello world" $ 
      ("./c-script",["./sample/test/test.cpp"],[]) @== 
        [str|hello world
            |]
    it "catch" $ 
      ("./c-script",["-no_chk_main","./sample/test/test-catch.cpp"],[]) @==
        [str|All tests passed (2 assertions in 1 test case)
            |
            |]
    it "source filter" $ 
      ("./c-script",["./sample/misc/source_filter_test.cpp"],[]) @==
        [str|3
            |]
  describe "basic-test" $ do
