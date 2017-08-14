(ns float_fuction.core
  (:gen-class)
  (:use seesaw.core))

(defn -main [& args]
  (invoke-later
    (-> (frame :title "Kakapos",
               :content "",
               :on-close :exit)
        pack!
        show!)))